//
// Created by agustin on 11/11/21.
//

#ifndef BLINK_DEVICE_H
#define BLINK_DEVICE_H

#include <monitor.h>


typedef enum {
    CAMARA,
    PUERTA,
    BOTON,
    CONSULTA_MQTT
} device_tipo_t;


typedef struct {
    device_tipo_t tipo;
    monitor_t *monitor;
    pthread_t t;
    pthread_attr_t attr;
    int enabled;
    _Noreturn void* (*tarea)(void *arg);
} device_t;

int device_init(device_t *d, monitor_t *m, device_tipo_t c);

int device_enable(device_t *d);

#endif //BLINK_DEVICE_H
