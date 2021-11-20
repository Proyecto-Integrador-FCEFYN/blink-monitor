//
// Created by agustin on 13/11/21.
//

#include "cerradura_device.h"
#include <sys/cdefs.h>
#include "driver/gpio.h"


#define GPIO_OUTPUT_IO_0    4 // FLASH

//#define GPIO_OUTPUT_PIN_SEL  ((1ULL<<GPIO_OUTPUT_IO_0) | (1ULL<<GPIO_OUTPUT_IO_1))
#define GPIO_OUTPUT_PIN_SEL  ((1ULL<<GPIO_OUTPUT_IO_0))

_Noreturn void *gpio_task_cerradura(void* arg)
{
    device_t *self = arg;
    while(1)
    {
    //T6, T3, T4
   // self->monitor->disparar(self->monitor,7);
   printf("hola!\n");
    sleep(1);
   // self->monitor->disparar(self->monitor,8);
   // sleep(1);
  //  self->monitor->disparar(self->monitor,9);
   // sleep(1);
    }
}


void cerradura_device_init(device_t *d, monitor_t *m)
{
    // GPIO
    gpio_config_t io_conf = {}; //zero-initialize the config structure.

    io_conf.intr_type = GPIO_INTR_DISABLE; //disable interrupt
    io_conf.mode = GPIO_MODE_OUTPUT; //set as output mode
    io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL; //bit mask of the pins that you want to set,e.g.GPIO18/19
    io_conf.pull_down_en = 0; //disable pull-down mode
    io_conf.pull_up_en = 0; //disable pull-up mode
    gpio_config(&io_conf); //configure GPIO with the given settings
}
