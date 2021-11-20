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
#include "monitor.h"
#include "procesador_petri.h"
#include "devices/device.h"


#include "sdkconfig.h"

#define _REENTRANT
#define _POSIX_PRIORITY_SCHEDULING


//_Noreturn void *tarea(void *arg);




void app_main(void)
{
    monitor_t monitor;
    procesador_petri_t petri;
    procesador_de_petri_init(&petri);
    monitor_init(&monitor, &petri);

    //ACA EL ORDEN IMPORTA POR COMO SE ACOMODA EL HEAP.
    //DEBE SER EL PRIMER DISPOSITIVO EN INICIAR!
    device_t cam_device;
    device_init(&cam_device, &monitor,CAMARA);

    device_t boton_device;
    device_init(&boton_device, &monitor, BOTON);
//    device_t cerradura_device;
//    device_init(&cerradura_device, &monitor, PUERTA);
///Muy importante que esta función no muera.
    while (1)
    {
            printf("ok!\n");
        vTaskDelay(1000 / portTICK_RATE_MS);
    }

//    long int i;
//    pthread_attr_t atrib;
//    pthread_t c[TRANSICIONES];
//    atrib.contentionscope = PTHREAD_SCOPE_SYSTEM;
//    atrib.schedpolicy = SCHED_RR;
//    pthread_attr_init(&atrib);
//
//    for (i=0;i<TRANSICIONES;i++)
//    {
//        pthread_create(&c[i], &atrib, tarea, (void *) i);
//    }
//    for (i=0;i<TRANSICIONES;i++)
//    {
//        pthread_join(c[i], NULL);
//    }

}

//_Noreturn void *tarea(void *arg)
//{
//    long int miid = (long int) arg;
//    while (1)
//    {
//        monitor.disparar(&monitor,(int) miid);
//    }
//}
