//
// Created by agustin on 11/11/21.
//

#include "device.h"
#include "gpio_device.h"
#include "cerradura_device.h"
#include "cam.h"


int device_init(device_t *d, monitor_t *m, device_class_t c)
{
    switch (c)
    {
        case CAMARA:
            d->class = CAMARA;
            init_camera(d, m);
            d->tarea = camera_task;
            break;
        case PUERTA:
            d->class = PUERTA;
            cerradura_device_init(d, m);
            d->tarea = gpio_task_cerradura;
            break;
        case BOTON:
            d->class = BOTON;
            gpio_device_init(d, m);
            d->tarea = gpio_task_boton;
            break;
        case CONSULTA_MQTT:
            d->class = CONSULTA_MQTT;
            //TODO: Init de MQTT
            //TODO: Task MQTT
            break;
        default:
            printf("Error en tipo de dispositivo\n");
    }

    d->monitor = m;

    // Atributos del thread.
    d->attr.contentionscope = PTHREAD_SCOPE_SYSTEM;
    d->attr.schedpolicy = SCHED_RR;
    pthread_attr_init(&d->attr);
    pthread_create(&d->t, &d->attr, d->tarea, (void *) d);

    return 0;
}
