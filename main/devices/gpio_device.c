#include <sys/queue.h>
#include <sys/cdefs.h>
//
// Created by agustin on 8/11/21.
//

#include "gpio_device.h"

/* GPIO Example
   This example code is in the Public Domain (or CC0 licensed, at your option.)
   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"


#define GPIO_INPUT_IO_0     0
#define GPIO_INPUT_PIN_SEL  ((1ULL<<GPIO_INPUT_IO_0) )
#define ESP_INTR_FLAG_DEFAULT 0

static xQueueHandle gpio_evt_queue = NULL;

static void IRAM_ATTR gpio_isr_handler_boton(void* arg)
{
    // T7, T8, T9
    uint32_t gpio_num = (uint32_t) arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

_Noreturn void* gpio_task_boton(void* arg)
{
    uint32_t io_num;
    for(;;) {
        if(xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) {
            printf("GPIO[%d] intr, val: %d\n", io_num, gpio_get_level(io_num));
        }
    }
}

void gpio_device_init(device_t *d, monitor_t *m)
{

    gpio_config_t io_conf = {}; //zero-initialize the config structure.

    // GPIO INPUT
    io_conf.intr_type = GPIO_INTR_POSEDGE; //interrupt of rising edge
    io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL; //bit mask of the pins, use GPIO4/5 here
    io_conf.mode = GPIO_MODE_INPUT; //set as input mode
    io_conf.pull_up_en = 1; //enable pull-up mode
    io_conf.pull_down_en = 0; //disable pull-down mode
    gpio_config(&io_conf);

    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t)); //create a queue to handle gpio event from isr

//    xTaskCreate(gpio_task_boton, "gpio_task_boton", 2048, NULL, 10, NULL); //start gpio task
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT); //install gpio isr service

    gpio_isr_handler_add(GPIO_INPUT_IO_0, gpio_isr_handler_boton, (void *) GPIO_INPUT_IO_0); //hook isr handler for specific gpio pin

}
