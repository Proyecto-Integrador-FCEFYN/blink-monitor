//
// Created by agustin on 21/11/21.
//

#ifndef BLINK_MQTT_HANDLER_H
#define BLINK_MQTT_HANDLER_H

#include "monitor.h"

typedef struct {
    monitor_t *monitor;
    char *buffer;
    unsigned int buffer_size;
    int enabled;
} mqtt_handler_t;

void mqtt_handler_init(mqtt_handler_t *h, monitor_t *m);

_Noreturn void *mqtt_handler_task(void* arg);

#endif //BLINK_MQTT_HANDLER_H
