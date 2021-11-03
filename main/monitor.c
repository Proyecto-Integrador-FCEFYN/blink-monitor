#include "monitor.h"
#include "macros.h"


static int logueo(int disparo)
{
    FILE *f = fopen("../file.txt", "a" );
    if (f == NULL)
    {
        printf("Error opening file!\n");
        return -1;
    }
    fprintf(f,"%i\n", disparo);
    printf("%i\n",disparo);
    fclose(f);
    return 1;
}

void monitor_disparar2(monitor_t *monitor, int disparo)
{
    pthread_mutex_lock(&monitor->entrada);

    int k = monitor->petri->solicitud_disparo(monitor->petri, disparo);
//  if (k == 0) // 0 si no se pudo disparar
    while (k == 0)
    {
#if DEBUG
        printf("No Sensibilizada: %i -- espera\n", disparo);
#endif
        pthread_cond_wait(&monitor->condition[disparo], &monitor->entrada);
        k = monitor->petri->solicitud_disparo(monitor->petri, disparo);
    }
    monitor->petri->disparar(monitor->petri, disparo);
#if LOG
    logueo(disparo);
#endif
#if DEBUG
    printf("Si sensibilizada: %i -- disparo\n",disparo);
        monitor->petri->toString(monitor->petri);
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


