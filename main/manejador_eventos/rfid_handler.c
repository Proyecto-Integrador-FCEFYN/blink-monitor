//
// Created by agustin on 28/04/22.
//
#include "freertos/FreeRTOS.h"
#include <esp_timer.h>
#include <freertos/queue.h>
#include <driver/uart.h>
#include <string.h>
#include <esp_log.h>
#include <esp_http_client.h>
#include "rfid_handler.h"
#include "macros.h"

#define DEBOUNCE_TIME  100 // the debounce-time in millisecond, increase this time if it still chatters

// RFID DATA FRAME FORMAT: 
// 1byte head (value: 2), 10byte data (2byte version + 8byte tag), 2byte checksum, 1byte tail (value: 3)
#define DATA_SIZE 10 // 10byte data (2byte version + 8byte tag)
#define DATA_VERSION_SIZE 2 // 2byte version (actual meaning of these two bytes may vary)
#define DATA_TAG_SIZE 8 // 8byte tag
#define CHECKSUM_SIZE 2 // 2byte checksum
#define eos(s) ((s)+strlen(s))

static const char *TAG = "RFID";
static const uart_port_t uart_num = UART_NUM_2;


void rfid_handler_init(rfid_handler_t *self, monitor_t *monitor)
{
    ESP_LOGI(TAG, "INGRESO A INIT RFID");
    uart_config_t uart_config = {
            .baud_rate = 9600,
            .data_bits = UART_DATA_8_BITS,
            .parity = UART_PARITY_DISABLE,
            .stop_bits = UART_STOP_BITS_1,
            .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
            .rx_flow_ctrl_thresh = 122,
    };
// Configure UART parameters
    ESP_ERROR_CHECK(uart_param_config(uart_num, &uart_config));
    // Set UART pins(TX: IO4, RX: IO5, RTS: IO18, CTS: IO19)
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM_2,
                                 -1,
                                 RFID_RX_PORT,
                                 UART_PIN_NO_CHANGE,
                                 UART_PIN_NO_CHANGE));
    // Setup UART buffered IO with event queue
    const int uart_buffer_size = (1024 * 2);
    // Install UART driver using an event queue here
    ESP_ERROR_CHECK(uart_driver_install(UART_NUM_2, uart_buffer_size, \
                                        uart_buffer_size, 10, NULL, 0));

    self->monitor = monitor;
    self->enabled = 0;
    self->buffer = "Not_init";
    self->buffer_size = 14;

    // Atributos del thread.
    pthread_attr_t attr;
    attr.contentionscope = PTHREAD_SCOPE_SYSTEM;
    attr.schedpolicy = SCHED_RR;
    pthread_attr_init(&attr);
    //Creacion de thread
    pthread_t t;
    pthread_create(&t, &attr, rfid_handler_task, (void *) self);
    ESP_LOGI(TAG, "UART INICIALIZADO!!!!!");
}

long hexstr_to_value(char *str, unsigned int length) { // converts a hexadecimal value (encoded as ASCII string) to a numeric value
 char* copy = malloc((sizeof(char) * length) + 1);
 memcpy(copy, str, sizeof(char) * length);
 copy[length] = '\0';
 // the variable "copy" is a copy of the parameter "str". "copy" has an additional '\0' element to make sure that "str" is null-terminated.
 long value = strtol(copy, NULL, 16); // strtol converts a null-terminated string to a long value
 free(copy); // clean up
 return value;
}

unsigned extract_tag(uint8_t *buffer) {
    char print_buf[256] = {0};
    uint8_t msg_head = buffer[0];
    uint8_t *msg_data = buffer + 1; // 10 byte => data contains 2byte version + 8byte tag
    uint8_t *msg_data_version = msg_data;
    uint8_t *msg_data_tag = msg_data + 2;
    uint8_t *msg_checksum = buffer + 11; // 2 byte
    uint8_t msg_tail = buffer[13];
    if ( msg_head != 0x2 || msg_tail != 0x3 ) {
        return -1;
    }
    // print message that was sent from RDM630/RDM6300
    sprintf(eos(print_buf), "--------\n");
    sprintf(eos(print_buf), "Message-Head: ");
    sprintf(eos(print_buf), "%x",msg_head);
    sprintf(eos(print_buf), "\n");
    sprintf(eos(print_buf), "Message-Data (HEX): \n");
    for (int i = 0; i < DATA_VERSION_SIZE; ++i) {
        sprintf(eos(print_buf), "%c", msg_data_version[i]);
    }
    sprintf(eos(print_buf), " (version)\n");
    for (int i = 0; i < DATA_TAG_SIZE; ++i) {
        sprintf(eos(print_buf), "%c", msg_data_tag[i]);
    }
    sprintf(eos(print_buf), " (tag)\n");
    sprintf(eos(print_buf), "Message-Checksum (HEX): ");
    for (int i = 0; i < CHECKSUM_SIZE; ++i) {
        sprintf(eos(print_buf), "%c", msg_checksum[i]);
    }
    sprintf(eos(print_buf), "\n");
    sprintf(eos(print_buf), "Message-Tail: ");
    sprintf(eos(print_buf), "%x",msg_tail);
    sprintf(eos(print_buf), "\n--\n");
    long tag = hexstr_to_value((char*) msg_data_tag, DATA_TAG_SIZE);
    if (tag == 0) {
        return -1;
    }
    sprintf(eos(print_buf), "Extracted Tag: ");
    sprintf(eos(print_buf), "%li",tag);
    long checksum = 0;
    for (int i = 0; i < DATA_SIZE; i+= CHECKSUM_SIZE) {
        long val = hexstr_to_value((char*) msg_data + i, CHECKSUM_SIZE);
        checksum ^= val;
    }
    sprintf(eos(print_buf), "\nExtracted Checksum (HEX): ");
    sprintf(eos(print_buf), "%lx",checksum);
    if (checksum == hexstr_to_value((char*) msg_checksum, CHECKSUM_SIZE)) { // compare calculated checksum to retrieved checksum
        sprintf(eos(print_buf), " (OK)"); // calculated checksum corresponds to transmitted checksum!
    } else {
        sprintf(eos(print_buf), " (NOT OK)"); // checksums do not match
        return -1;
    }
    sprintf(eos(print_buf), "\n");
    sprintf(eos(print_buf), "--------\n");
    printf("%s",print_buf);
    return tag;
}

_Noreturn void* rfid_handler_task(void* arg)
{
    // rfid_handler_t *self = arg;
//    char RFIDcurrentState[14];

    // Variables will change:
    static char lastSteadyState[14] = {0};       // the previous steady state from the input pin
    static char lastFlickerableState[14] = {0};  // the previous flickerable state from the input pin

    // the following variables are unsigned longs because the time, measured in
    // milliseconds, will quickly become a bigger number than can be stored in an int.
    uint64_t lastDebounceTime = 0;  // the last time the output pin was toggled

    while(1)
    {
//        if(self->enabled)
        {
            // read the state of the switch/button:
            uart_read_bytes(uart_num, RFIDcurrentState, 14, 100); //14 es el largo del codigo

            // If the switch/button changed, due to noise or pressing:
            if (strncmp(RFIDcurrentState, lastFlickerableState, 14) != 0)
            {
                // reset the debouncing timer
                lastDebounceTime = esp_timer_get_time();
                // save the last flickerable state
                strncpy(lastFlickerableState, RFIDcurrentState, 14);
            }

            if ( (esp_timer_get_time() - lastDebounceTime) > DEBOUNCE_TIME)
            {
                // whatever the reading is at, it's been there for longer than the debounce time
                // delay, so take it as the actual current state:

                // if the button state has changed:
                if (strncmp(lastSteadyState, RFIDcurrentState, 14) != 0)
                {
                    /*self->monitor->disparar(self->monitor, T_EVENTO_RFID);
//                    self->buffer_size = 14;
//                    strncpy(self->buffer, RFIDcurrentState,14);*/
                    // HTTPS-------------------------------------------------------------------
                    extern const uint8_t localhost_pem_start[] asm("_binary_localhost_pem_start");
                    extern const uint8_t localhost_pem_end[]   asm("_binary_localhost_pem_end");

                    esp_http_client_config_t cfg = {
                        .url = API_BASE_URL"/api/v1/event/rfid",
                        .method = HTTP_METHOD_POST,
//                        .cert_pem = (const char *) localhost_pem_start,
                        .skip_cert_common_name_check = true
//                        .client_cert_pem = (const char *) localhost_pem_start
                    };
                    esp_http_client_handle_t client = esp_http_client_init(&cfg);

                    esp_http_client_set_header(client, "Content-Type", "application/json");

                    char data[32] = {0};    
                    int current_tag = extract_tag((uint8_t*) RFIDcurrentState);
                    if (current_tag > 0) // Para evitar las lecturas rnd de 0
                    {
                        snprintf(data, 32, "%i", current_tag);
                        ESP_LOGI(TAG, "SE CONSTRUYO EL BODY %s", data);
                        esp_http_client_set_post_field(client, data, strlen(data));
                        esp_http_client_perform(client);
                        esp_http_client_cleanup(client);
                        ESP_LOGI(TAG, "Llego nuevo codigo RFID");
                    }
                }
                // save the last steady state
                strncpy(lastSteadyState, RFIDcurrentState, 14);
            }
        } //enable
        vTaskDelay(100);
    } //while
}
