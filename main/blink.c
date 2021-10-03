/* Blink Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "pthread.h"

#include "sdkconfig.h"

#define REENTRANT

#define NUMREC 10
#define NUMPROC 5

pthread_mutex_t mutex;
pthread_cond_t espera;

int recdis = NUMREC;

void pidoRec (int n, int id) {
    pthread_mutex_lock(&mutex);
    while (recdis < n)
    {
        printf("Trabajador %d Core %d, pide %d −− espera\n", id, xPortGetCoreID(), n);
        pthread_cond_wait(&espera, &mutex);
    }
    recdis = recdis - n;
    printf("Trabajador %d Core %d, pide %d -- toma\n", id, xPortGetCoreID(), n);
    pthread_mutex_unlock(&mutex);
}

void devuelvoRec (int n, int id) {
    pthread_mutex_lock(&mutex);
    recdis = recdis + n;
    printf("Trabajador %d Core %d, devuelve %d\n", id, xPortGetCoreID(), n);
    pthread_cond_broadcast(&espera);
    pthread_mutex_unlock(&mutex);
}

void *cliente(void *arg) {
    int miid = (int) arg;
    int d, i;
    // for ( i = 0; i < 10; i++)
    while(1)
    {
        d = rand() % 10;
        pidoRec(d, miid);
        devuelvoRec(d, miid);
    }
    pthread_exit(NULL);
}


void app_main(void)
{
    int i = 0;
    pthread_attr_t atrib;
    pthread_t c[NUMPROC];

    pthread_attr_init(&atrib);
    // Ver si existen equivalencias para esto.
    // pthread_attr_setscope(&atrib,PTHREAD_SCOPE_SYSTEM);
    //i = pthread_attr_setschedpolicy(&atrib,SCHED_RR);
    printf("%d\n",i);

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&espera, NULL);
 
    for (i=0;i<5;i++)
    {
        pthread_create(&c[i], &atrib, cliente, (void*) i) ;
    }
    
    for (i=0;i<5;i++)
    {
        pthread_join(c[i], NULL);
    }



}
