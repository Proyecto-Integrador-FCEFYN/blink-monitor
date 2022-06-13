//
// Created by agustin on 13/11/21.
//
#ifndef BLINK_CERRADURA_DEVICE_H
#define BLINK_CERRADURA_DEVICE_H

#include <sys/cdefs.h>
#include <hal/gpio_types.h>

typedef struct
{
    gpio_num_t gpio_num;
    unsigned int estado;
    unsigned int timeout_limit;
    int timeout_count;
    int duracion_apertura;
} dev_cerradura_t;

void cerradura_device_init(dev_cerradura_t *d, int duracion_cerradura);

void cerradura_abrirpuerta(dev_cerradura_t *self);
void cerradura_esperar_timeout(dev_cerradura_t *self);

#endif //BLINK_CERRADURA_DEVICE_H
