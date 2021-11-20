#ifndef CLASE_GPIO_DEVICE_H
#define CLASE_GPIO_DEVICE_H

#include <sys/cdefs.h>
#include "device.h"
//
// Created by agustin on 8/11/21.
//


void gpio_device_init(device_t *d, monitor_t *m);

_Noreturn void* gpio_task_boton(void* arg);

#endif /* CLASE_GPIO_DEVICE_H */
