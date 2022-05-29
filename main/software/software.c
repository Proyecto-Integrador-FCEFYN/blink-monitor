//
// Created by agustin on 12/05/22.
//

#include "software.h"
#include <esp_log.h>

static const char *TAG = "SOFTWARE";


int seqdisparo_init(seqdisparo_t *self, unsigned int size, int *transiciones, void *actions)
{
    self->size = size;
    self->transiciones = transiciones;
    self->actions = actions;
    if (self->transiciones == NULL || self->actions == NULL)
    {
        ESP_LOGE(TAG, "Error en el malloc de secuencia de disparo");
        return -1;
    }


   return 0;
}



_Noreturn void *seqdisparo_run(void *arg)
{
    seqdisparo_t *self = arg;
    while (1)
    {
        for (int i = 0; i < self->size; ++i)
        {
            self->actions[i](self);
            self->monitor->disparar(self->monitor,i);
        }
    }
}

void software_init(software_t *self, seqdisparo_t *seqs)
{
    self->threads = malloc(HILOS * sizeof(pthread_t));
    self->seqdisparos = malloc(HILOS * sizeof(seqdisparo_t));

    pthread_attr_t attr;
    attr.contentionscope = PTHREAD_SCOPE_SYSTEM;
    attr.schedpolicy = SCHED_RR;
    pthread_attr_init(&attr);

    for (int i = 0; i < HILOS; ++i)
    {
        self->seqdisparos = seqs;
        pthread_create(&self->threads[i],
                       &attr,
                       seqdisparo_run,
                       (void*) &self->seqdisparos[i]);
    }
}

