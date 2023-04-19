//
// Created by agustin on 8/11/21.
//
#ifndef CLASE_MOVIM_HANDLER_H
#define CLASE_MOVIM_HANDLER_H

#include "monitor.h"

typedef struct {
    monitor_t *monitor;
    char *buffer;
    unsigned int buffer_size;
    int enabled;
} movimiento_handler_t;

void movimiento_handler_init(movimiento_handler_t *h, monitor_t *m);

void enviar_evento_movimiento();

_Noreturn void* movimiento_handler_task(void* arg);

#endif /* CLASE_MOVIM_HANDLER_H */
