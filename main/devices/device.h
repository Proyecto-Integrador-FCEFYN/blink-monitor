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
} device_class_t;

//typedef struct device_t device_t;

typedef struct {
    monitor_t *monitor;
    device_class_t class;
    void (*device_handler)(void);
    void (*tarea)(void *);
} device_t;

int device_init(device_t *d, monitor_t *m, device_class_t c); //agregar handlers

#endif //BLINK_DEVICE_H
