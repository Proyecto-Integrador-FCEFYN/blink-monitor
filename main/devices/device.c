//
// Created by agustin on 11/11/21.
//

#include "device.h"
#include "gpio_device.h"


int device_init(device_t *d, monitor_t *m, device_class_t c)
{
    d->monitor = m;

    switch (c)
    {
        case CAMARA:
            break;
        case PUERTA:
            break;
        case BOTON:
            gpio_device_init(d, m);
            break;
        case CONSULTA_MQTT:
            break;
        default:

            printf("Error en tipo de dispositivo\n");
    }

    return 0;
}
