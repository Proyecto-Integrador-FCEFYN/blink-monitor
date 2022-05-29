//
// Created by agustin on 11/11/21.
//

#ifndef BLINK_HANDLER_H
#define BLINK_HANDLER_H

#include <monitor.h>


typedef enum {
    BOTON,
    D_COMM,
    RFID
} handler_tipo_t;

typedef struct handler_t handler_t;

struct handler_t {
    handler_tipo_t tipo;
    monitor_t *monitor;
    char *buffer;
    unsigned int buffer_size;
    int enabled;
    void (*init)(handler_t *self);
    _Noreturn void* (*tarea)(void *arg);
};

int handler_init(handler_t *h, monitor_t *m, handler_tipo_t c);

int handler_enable(handler_t *h);

#endif //BLINK_HANDLER_H
