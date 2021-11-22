//
// Created by agustin on 8/11/21.
//

#include "boton_device.h"

#include <sys/cdefs.h>
#include "driver/gpio.h"
#include <stdio.h>
#include <esp_timer.h>
#include <esp_log.h>
#include "freertos/FreeRTOS.h"
#include <freertos/task.h>
#include <freertos/queue.h>

#define GPIO_INPUT_IO_0     2  // TODO: ELEGIR PIN PARA BOTON
#define GPIO_INPUT_PIN_SEL  ((1ULL<<GPIO_INPUT_IO_0) )
#define ESP_INTR_FLAG_DEFAULT 0

static const char *TAG = "BOTON";

static xQueueHandle gpio_evt_queue = NULL;

static void IRAM_ATTR gpio_isr_handler_boton(void* arg)
{
    printf("Entre al handler de boton\n");
    uint32_t pin = GPIO_INPUT_IO_0;
    xQueueSendFromISR(gpio_evt_queue, &pin, NULL);

}

#define BUTTON_PIN     2 // GIOP21 pin connected to button
#define DEBOUNCE_TIME  50 // the debounce time in millisecond, increase this time if it still chatters
#define LOW 0
#define HIGH 1

_Noreturn void* gpio_task_boton(void* arg)
{

    device_t *self = arg;

    // Variables will change:
    uint64_t lastSteadyState = LOW;       // the previous steady state from the input pin
    uint64_t lastFlickerableState = LOW;  // the previous flickerable state from the input pin
    uint64_t currentState;                // the current reading from the input pin

    // the following variables are unsigned longs because the time, measured in
    // milliseconds, will quickly become a bigger number than can be stored in an int.
    uint64_t lastDebounceTime = 0;  // the last time the output pin was toggled
    ESP_LOGI(TAG, "The button tiene variables");
    uint32_t io_num = GPIO_INPUT_IO_0;
    while(1)
    {
//        if(xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) {
//            printf("GPIO[%d] intr, val: %d\n", io_num, gpio_get_level(io_num));
//        }

      //  ESP_LOGI(TAG, "Dentro del while");
        if(self->enabled)
        {
//            if (gpio_get_level(GPIO_INPUT_IO_0) == 0)
//            {
//                 self->monitor->disparar(self->monitor, 7);
//                 ESP_LOGI(TAG, "T3\t SUENA EL TIMBREEEEEEEEE!");
//            }

            // read the state of the switch/button:
            currentState = gpio_get_level(BUTTON_PIN);

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

            if ( (esp_timer_get_time() - lastDebounceTime) > DEBOUNCE_TIME)
            {
                // whatever the reading is at, it's been there for longer than the debounce
                // delay, so take it as the actual current state:

                // if the button state has changed:
                if(lastSteadyState == HIGH && currentState == LOW)
                {
                    ESP_LOGI(TAG, "The button is pressed");
                    self->monitor->disparar(self->monitor, 7);
                }
                else if(lastSteadyState == LOW && currentState == HIGH)
                {
                    ESP_LOGI(TAG, "The button is released");
                }

                // save the last steady state
                lastSteadyState = currentState;
            }
        } //enable
        vTaskDelay(10);
    } //while
}// task

void gpio_device_init(device_t *d, monitor_t *m)
{

    gpio_config_t io_conf2 = {}; //zero-initialize the config structure.

    // GPIO INPUT
    io_conf2.intr_type = GPIO_INTR_POSEDGE; //interrupt of rising edge
    io_conf2.pin_bit_mask = GPIO_INPUT_PIN_SEL; //bit mask of the pins, use GPIO4/5 here
    io_conf2.mode = GPIO_MODE_INPUT; //set as input mode
    io_conf2.pull_up_en = 1; //enable pull-up mode
    io_conf2.pull_down_en = 0; //disable pull-down mode
    gpio_config(&io_conf2);

    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t)); //create a queue to handle gpio event from isr

//    xTaskCreate(gpio_task_boton, "gpio_task_boton", 2048, NULL, 10, NULL); //start gpio task
//    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT); //install gpio isr service
//    gpio_isr_handler_add(GPIO_INPUT_IO_0,gpio_isr_handler_boton, (void *) d);


}
