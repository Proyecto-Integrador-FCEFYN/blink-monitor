#include <esp_log.h>
#include "monitor.h"
#include "macros.h"
#include "boton_device.h"

static const char* TAG = "Monitor";



void monitor_disparar2(monitor_t *monitor, int disparo)
{
    pthread_mutex_lock(&monitor->entrada);

    int k = monitor->petri->solicitud_disparo(monitor->petri, disparo);
//  if (k == 0) // 0 si no se pudo disparar
    while (k == 0)
    {
#if DEBUG
        ESP_LOGW(TAG, "No Sensibilizada: %i -- espera\n", disparo);
        //printf("No Sensibilizada: %i -- espera\n", disparo);
#endif
        pthread_cond_wait(&monitor->condition[disparo], &monitor->entrada);
        k = monitor->petri->solicitud_disparo(monitor->petri, disparo);
    }
    monitor->petri->disparar(monitor->petri, disparo);

#if DEBUG
        ESP_LOGW(TAG, "Si sensibilizada: %i -- disparo\n",disparo);
    // printf("Si sensibilizada: %i -- disparo\n",disparo);
    // monitor->petri->toString(monitor->petri);
#endif
    for (int i = 0; i < TRANSICIONES; ++i)
    {
//            pthread_cond_broadcast(&monitor->condition[i]);
        pthread_cond_signal(&monitor->condition[i]);
    }
    pthread_mutex_unlock(&monitor->entrada);
}

void monitor_init(monitor_t *monitor, procesador_petri_t *petri)
{
    pthread_mutex_init(&monitor->entrada, NULL);
    for (int i = 0; i < TRANSICIONES; ++i)
    {
        pthread_cond_init(&monitor->condition[i], NULL);
    }
    monitor->petri = petri;
    monitor->disparar = monitor_disparar2;
}


