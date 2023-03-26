//
// Created by agustin on 13/11/21.
//

#include "cerradura_device.h"

#include <sys/cdefs.h>
#include <esp_log.h>
#include "freertos/FreeRTOS.h"
#include <freertos/task.h>
#include "driver/gpio.h"
#include "macros.h"
#include <unistd.h>

static const char *TAG = "CERRADURA";

void cerradura_abrirpuerta(dev_cerradura_t *self)
{
    gpio_set_level(CERRADURA_GPIO, 1);
    ESP_LOGI(TAG,"PUERTA ABIERTA!");
    self->estado = gpio_get_level(CERRADURA_GPIO);
    sleep(CERRADURA_ABIERTA);
    gpio_set_level(CERRADURA_GPIO, 0);
    ESP_LOGI(TAG,"PUERTA CERRADA!");
    self->estado = gpio_get_level(CERRADURA_GPIO);
    self->timeout_count = -1;
}

void cerradura_device_init(dev_cerradura_t *self, int duracion_apertura)
{
    self->gpio_num = CERRADURA_GPIO;
    self->timeout_limit = CERRADURA_TIMEOUT;
    self->timeout_count = -1;
    self->estado = 0;
    self->duracion_apertura = duracion_apertura;

    // GPIO
    gpio_config_t io_conf = {}; //zero-initialize the config structure.
    io_conf.intr_type = GPIO_INTR_DISABLE; //disable interrupt
    io_conf.mode = GPIO_MODE_OUTPUT; //set as output mode
    io_conf.pin_bit_mask = 1<<CERRADURA_GPIO; //bit mask of the pins that you want to set,e.g.GPIO18/19
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE; //ensable pull-down mode
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE; //enable pull-up mode
    gpio_config(&io_conf); //configure GPIO with the given settings
    //
    ESP_ERROR_CHECK(gpio_set_level(CERRADURA_GPIO, 1));
    // self->estado = gpio_get_level(CERRADURA_GPIO);
}
