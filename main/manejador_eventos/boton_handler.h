//
// Created by agustin on 8/11/21.
//
#ifndef CLASE_GPIO_HANDLER_H
#define CLASE_GPIO_HANDLER_H

#include "monitor.h"

typedef struct {
    monitor_t *monitor;
    char *buffer;
    unsigned int buffer_size;
    int enabled;
} boton_handler_t;

void boton_handler_init(boton_handler_t *h, monitor_t *m);

void enviar_evento_boton(); 

_Noreturn void* boton_handler_task(void* arg);

#endif /* CLASE_GPIO_HANDLER_H */
