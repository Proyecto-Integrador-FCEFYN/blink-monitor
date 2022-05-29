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

typedef int (*action_p)(void *self);

typedef struct seqdisparo
{
    int *transiciones;
    action_p *actions;
    unsigned int size;
    monitor_t *monitor;
} seqdisparo_t;


typedef struct software
{
    seqdisparo_t *seqdisparos;
    pthread_t *threads;
    // Manejador de errores para los errores dentro de las secuencias.
} software_t;

void software_init(software_t *self, seqdisparo_t *seqs);

int seqdisparo_init(seqdisparo_t *self, unsigned int size, int *transiciones, void *actions);

#endif //BLINK_SOFTWARE_H
