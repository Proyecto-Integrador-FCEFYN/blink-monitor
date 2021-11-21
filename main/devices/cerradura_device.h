//
// Created by agustin on 13/11/21.
//
#ifndef BLINK_CERRADURA_DEVICE_H
#define BLINK_CERRADURA_DEVICE_H

#include <sys/cdefs.h>
#include "device.h"

void cerradura_device_init(device_t *d, monitor_t *m);

_Noreturn void* gpio_task_cerradura(void* arg);

#endif //BLINK_CERRADURA_DEVICE_H
