//
// Created by agustin on 12/05/22.
//

/// Supuestos:
/// * La cantidad de secuencias es igual a la cantidad de hilos.
/// * Las secuencias intercalan acciones y transiciones secuenciales.
/// * La cantidad de acciones y de transiciones dentro de una secuencia
///   siempre son iguales.

#ifndef BLINK_SOFTWARE_H
#define BLINK_SOFTWARE_H

#include "macros.h"
#include "pthread.h"
#include "comm_dev.h"
#include "cerradura_device.h"
#include "cam.h"

typedef void (*action_p)(void *self);
typedef void *objeto_t;

typedef struct segmento
{
    int *secuencia;
    unsigned int segmento_size;
    action_p *actions;
    monitor_t *monitor;
    objeto_t *objetos;
} segmento_t;


typedef struct software
{
    segmento_t *segmentos;
    pthread_t *threads;
    // Manejador de errores para los errores dentro de las secuencias.
} software_t;

void software_init(software_t *self, segmento_t *segmentos);

int segmento_init(segmento_t *self,
                  monitor_t *monitor,
                  int *secuencia_transiciones,
                  void *actions,
                  objeto_t *objetos,
                  unsigned int segmento_size);

#endif //BLINK_SOFTWARE_H
