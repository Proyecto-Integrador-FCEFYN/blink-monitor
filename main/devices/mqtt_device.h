//
// Created by agustin on 16/11/21.
//

#ifndef BLINK_MQTT_DEVICE_H
#define BLINK_MQTT_DEVICE_H


#include "device.h"

void mqtt_device_init(device_t *d, monitor_t *m);

_Noreturn void* mqtt_device_task(void* arg);

#endif //BLINK_MQTT_DEVICE_H
