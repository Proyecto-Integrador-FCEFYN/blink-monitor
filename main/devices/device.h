//
// Created by agustin on 11/11/21.
//

#ifndef BLINK_DEVICE_H
#define BLINK_DEVICE_H

#include <monitor.h>

//// ----------- DEVICE -----------------
enum device_class{
    CAMARA,
    PUERTA,
    BOTON,
    CONSULTA_MQTT
};

struct device_t {
    monitor_t *monitor;
    enum device_class class;
    void (*device_handler)(void);
};

int device_init(monitor_t *monitor, enum device_class class); //agregar handlers

#endif //BLINK_DEVICE_H
