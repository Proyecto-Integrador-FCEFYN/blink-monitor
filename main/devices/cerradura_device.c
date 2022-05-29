//
// Created by agustin on 13/11/21.
//

#include "cerradura_device.h"

#include <sys/cdefs.h>
#include <esp_log.h>
#include "freertos/FreeRTOS.h"
#include <freertos/task.h>
#include "driver/gpio.h"
#include "comm_dev.h"

static const char *TAG = "CERRADURA";

void cerradura_abrirpuerta(dev_cerradura_t *self)
{
    gpio_set_level(self->gpio_num, 1);
    self->estado = gpio_get_level(self->gpio_num);
    sleep(2);
    gpio_set_level(self->gpio_num, 0);
    self->estado = gpio_get_level(self->gpio_num);
    self->timeout_count = -1;
}

void cerradura_esperar_timeout(dev_cerradura_t *self)
{
    self->timeout_count = 0;
    for (int i = 0; i<self->timeout_limit; ++i)
    {
        sleep(1);
        self->timeout_count++;
        if (self->timeout_count < 0)
        {
            break;
        }
    }
    ESP_LOGI(TAG,"Se llego al limite de timeout");
}


void cerradura_device_init(device_t *self)
{
    dev_cerradura_t *this_cerradura_device = self->context;
    this_cerradura_device->gpio_num = CERRADURA_GPIO;
    this_cerradura_device->timeout_limit = CERRADURA_TIMEOUT;
    this_cerradura_device->timeout_count = 0;
    // GPIO
    gpio_config_t io_conf = {}; //zero-initialize the config structure.
    io_conf.intr_type = GPIO_INTR_DISABLE; //disable interrupt
    io_conf.mode = GPIO_MODE_OUTPUT; //set as output mode
    io_conf.pin_bit_mask = 1<<this_cerradura_device->gpio_num; //bit mask of the pins that you want to set,e.g.GPIO18/19
    io_conf.pull_down_en = 0; //disable pull-down mode
    io_conf.pull_up_en = 0; //disable pull-up mode
    gpio_config(&io_conf); //configure GPIO with the given settings
    //
    gpio_set_level(this_cerradura_device->gpio_num, 0);
    this_cerradura_device->estado = gpio_get_level(this_cerradura_device->gpio_num);
}
