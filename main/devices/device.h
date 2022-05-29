//
// Created by agustin on 11/11/21.
//

#ifndef BLINK_HANDLER_H
#define BLINK_HANDLER_H

#include <monitor.h>


typedef enum {
    D_CAMARA,
    D_PUERTA,
    D_COMM
} device_tipo_t;

typedef struct device_t device_t;

struct device_t {
    device_tipo_t tipo;
    void *context;
    void (*init)(device_t *self);
};

int device_init(device_t *d, device_tipo_t c);

#endif //BLINK_HANDLER_H
