//
// Created by agustin on 8/11/21.
//

#include "boton_handler.h"

#include <sys/cdefs.h>
#include "driver/gpio.h"
//#include <stdio.h>
#include <esp_timer.h>
#include <esp_log.h>
#include "freertos/FreeRTOS.h"
#include <freertos/task.h>
#include <string.h>
#include <esp_http_client.h>
#include "macros.h"
#include "esp_tls.h"
//#define GPIO_INPUT_IO_0     2  // TODO: ELEGIR PIN PARA BOTON
//#define GPIO_INPUT_PIN_SEL  ((1ULL<<GPIO_INPUT_IO_0) )

static const char *TAG = "BOTON_HANDLER";




//#define DEBOUNCE_TIME  50 // the debounce-time in millisecond, increase this time if it still chatters
#define LOW 0
#define HIGH 1

void enviar_evento_boton() 
{
    ESP_LOGI(TAG, "Envio evento boton");
}

_Noreturn void* boton_handler_task(void* arg)
{

    boton_handler_t *self = arg;
    // Variables will change:
    uint64_t lastSteadyState = LOW;       // the previous steady state from the input pin
    uint64_t lastFlickerableState = LOW;  // the previous flickerable state from the input pin
    uint64_t currentState;                // the current reading from the input pin

    // the following variables are unsigned longs because the time, measured in
    // milliseconds, will quickly become a bigger number than can be stored in an int.
    uint64_t lastDebounceTime = 0;  // the last time the output pin was toggled
//    ESP_LOGI(TAG, "The button tiene variables");
    while(1)
    {
        //  ESP_LOGI(TAG, "Dentro del while");
        if(self->enabled)
        {

            // read the state of the switch/button:
            currentState = gpio_get_level(BOTON_GPIO);

            // check to see if you just pressed the button
            // (i.e. the input went from LOW to HIGH), and you've waited long enough
            // since the last press to ignore any noise:

            // If the switch/button changed, due to noise or pressing:
            if (currentState != lastFlickerableState) {
                // reset the debouncing timer
                lastDebounceTime = esp_timer_get_time();
                // save the last flickerable state
                lastFlickerableState = currentState;
            }

            if ( (esp_timer_get_time() - lastDebounceTime) > BOTON_DEBOUNCE_TIME)
            {
                // whatever the reading is at, it's been there for longer than the debounce time
                // delay, so take it as the actual current state:

                // if the button state has changed:
                if(lastSteadyState == HIGH && currentState == LOW)
                {
//                    self->monitor->disparar(self->monitor, T_EVENTO_BOTON);
//                    char *msj = "ON";
//                    self->buffer_size = strlen(msj);
//                    self->buffer = msj;

                    // HTTPS-------------------------------------------------------------------
                    extern const uint8_t localhost_pem_start[] asm("_binary_localhost_pem_start");
                    // extern const uint8_t localhost_pem_end[]   asm("_binary_localhost_pem_end");

                    esp_http_client_config_t cfg = {
                        .url = API_BASE_URL"/api/v1/event/timbre",
                        .method = HTTP_METHOD_POST,
                        .auth_type = HTTP_AUTH_TYPE_BASIC,
                        .username = API_USERNAME,
                        .password = API_PASSWORD,
                        .skip_cert_common_name_check = true,
                        .cert_pem = (const char *) localhost_pem_start
                    };
                    esp_http_client_handle_t client = esp_http_client_init(&cfg);
                    esp_http_client_perform(client);
                    esp_http_client_cleanup(client);

                    ESP_LOGI(TAG, "The button is pressed");
                }
                else if(lastSteadyState == LOW && currentState == HIGH)
                {
                    char *msj = "OFF";
                    self->buffer_size = strlen(msj);
                    self->buffer = msj;
                    ESP_LOGI(TAG, "The button is released");
                }

                // save the last steady state
                lastSteadyState = currentState;
            }
        } //enable
        vTaskDelay(10);
    } //while
}// task

void boton_handler_init(boton_handler_t *self, monitor_t *m)
{

    gpio_config_t io_conf2 = {}; //zero-initialize the config structure.

    // GPIO INPUT
    io_conf2.intr_type = GPIO_INTR_DISABLE; //interrupt of rising edge
    io_conf2.pin_bit_mask = 1<<BOTON_GPIO; //bit mask of the pins, use GPIO4/5 here
    io_conf2.mode = GPIO_MODE_INPUT; //set as input mode
    io_conf2.pull_up_en = 1; //enable pull-up mode
    io_conf2.pull_down_en = 0; //disable pull-down mode
    gpio_config(&io_conf2);

    self->monitor = m;
    self->enabled = 0;
    self->buffer = "";
    self->buffer_size = 0;

    // Atributos del thread.
    pthread_attr_t attr;
    attr.contentionscope = PTHREAD_SCOPE_SYSTEM;
    attr.schedpolicy = SCHED_RR;
    pthread_attr_init(&attr);
    //Creacion de thread
    pthread_t t;
    pthread_create(&t, &attr, boton_handler_task, (void *) self);
}
