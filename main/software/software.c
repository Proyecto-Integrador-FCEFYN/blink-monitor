//
// Created by agustin on 12/05/22.
//

#include "software.h"
#include <esp_log.h>

static const char *TAG = "SOFTWARE";


int segmento_init(segmento_t self[],
                  monitor_t *monitor,
                  int *secuencia_transiciones,
                  void *actions,
                  objeto_t *objetos,
                  unsigned int segmento_size)
{
    self->monitor = monitor;
    self->secuencia = secuencia_transiciones;
    self->actions = actions;
    self->objetos = objetos;
    self->segmento_size = segmento_size;
    if (self->secuencia == NULL ||
        self->actions == NULL ||
        self->objetos == NULL)
    {
        ESP_LOGE(TAG, "Error en el init de segmento");
        return -1;
    }
    return 0;
}



_Noreturn void *segmento_run(void *arg)
{
    segmento_t *self = arg;
    while (1)
    {
        for (int i = 0; i < self->segmento_size; ++i)
        {
            if(self->secuencia[i] != NULL_TRANSITION)
            {
                self->monitor->disparar(self->monitor,self->secuencia[i]);
            }
            if (self->actions[i] != NULL || self->objetos[i] != NULL)
            {
                self->actions[i](&self->objetos[i]);
            }
        }
        vTaskDelay(10);
    }
}

void software_init(software_t *self, segmento_t *segmentos)
{
    self->threads = malloc(HILOS * sizeof(pthread_t));
    self->segmentos = malloc(HILOS * sizeof(segmento_t));
    self->segmentos = segmentos;

    pthread_attr_t attr;
    attr.contentionscope = PTHREAD_SCOPE_SYSTEM;
    attr.schedpolicy = SCHED_RR;
    pthread_attr_init(&attr);

    for (int i = 0; i < HILOS; ++i)
    {
        pthread_create(&self->threads[i],
                       &attr,
                       segmento_run,
                       (void *) &self->segmentos[i]);
        ESP_LOGI(TAG,"CREACION DEL HILO %i",i);
    }
}

