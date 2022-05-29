//
// Created by agustin on 11/11/21.
//

#include "handler.h"
#include "boton_handler.h"
#include "rfid_handler.h"
#include "mqtt_handler.h"


int handler_init(handler_t *h, monitor_t *m, handler_tipo_t c)
{
    switch (c)
    {
        case BOTON:
            h->tipo = BOTON;
            h->init = boton_handler_init;
            h->tarea = boton_handler_task;
            break;
        case D_COMM:
            h->tipo = D_COMM;
            h->init = mqtt_handler_init;
            h->tarea = mqtt_handler_task;
            break;
        case RFID:
            h->tipo = RFID;
            h->init = rfid_handler_init;
            h->tarea = rfid_handler_task;
            break;
        default:
            printf("Error en tipo de dispositivo\n");
    }

    h->monitor = m;
    h->enabled = 0;

    h->init(h);

    // Atributos del thread.
    pthread_attr_t attr;
    attr.contentionscope = PTHREAD_SCOPE_SYSTEM;
    attr.schedpolicy = SCHED_RR;
    pthread_attr_init(&attr);
    //Creacion de thread
    pthread_t t;
    pthread_create(&t, &attr, h->tarea, (void *) h);

    return 0;
}

int handler_enable(handler_t *h){
    h->enabled = 1;
    return 0;
}
