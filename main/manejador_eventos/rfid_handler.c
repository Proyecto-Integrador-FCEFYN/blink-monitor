//
// Created by agustin on 28/04/22.
//
#include "freertos/FreeRTOS.h"
#include <freertos/queue.h>
#include <driver/uart.h>
#include <string.h>
#include <esp_log.h>
#include "rfid_handler.h"
#include "macros.h"

#define DEBOUNCE_TIME  100 // the debounce-time in millisecond, increase this time if it still chatters

static const char *TAG = "RFID";
static const uart_port_t uart_num = UART_NUM_2;

void rfid_handler_init(handler_t *h)
{
    h->buffer_size = 14;

    /* Configure the IOMUX register for pad BLINK_GPIO (some pads are
      muxed to GPIO on reset already, but some default to other
      functions and need to be switched to GPIO. Consult the
      Technical Reference for a list of pads and their default
      functions.)
   */
//
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
                                 12,
                                 13,
                                 UART_PIN_NO_CHANGE,
                                 UART_PIN_NO_CHANGE));
    // Setup UART buffered IO with event queue
    const int uart_buffer_size = (1024 * 2);
    QueueHandle_t uart_queue;
    // Install UART driver using an event queue here
    ESP_ERROR_CHECK(uart_driver_install(UART_NUM_2, uart_buffer_size, \
                                        uart_buffer_size, 10, &uart_queue, 0));

}

_Noreturn void* rfid_handler_task(void* arg)
{
    handler_t *self = arg;
    char RFIDcurrentState[14];

     // Variables will change:
    static char lastSteadyState[14] = {0};       // the previous steady state from the input pin
    static char lastFlickerableState[14] = {0};  // the previous flickerable state from the input pin

    // the following variables are unsigned longs because the time, measured in
    // milliseconds, will quickly become a bigger number than can be stored in an int.
    uint64_t lastDebounceTime = 0;  // the last time the output pin was toggled

    while(1)
     {
        if(self->enabled)
        {
            // read the state of the switch/button:
            uart_read_bytes(uart_num, RFIDcurrentState, 14, 100); //14 es el largo del codigo


            // check to see if you just pressed the button
            // (i.e. the input went from LOW to HIGH), and you've waited long enough
            // since the last press to ignore any noise:

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
//                if(lastSteadyState == HIGH && RFIDcurrentState == LOW)
                {
                    self->monitor->disparar(self->monitor, T_EVENTO_RFID);
                    self->buffer_size = 14;
                    strncpy(self->buffer, RFIDcurrentState,14);
                    ESP_LOGI(TAG, "NEW VALUE READ!");
                }
//                else if (strncmp(lastSteadyState, RFIDcurrentState, 14) == 0)
//                else if(lastSteadyState == LOW && RFIDcurrentState == HIGH)
//                {
//                    ESP_LOGI(TAG, "The button is released");
//                }

                // save the last steady state
                strncpy(lastSteadyState, RFIDcurrentState, 14);
            }
        } //enable
        vTaskDelay(10);
    } //while
}