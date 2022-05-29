//
// Created by agustin on 11/11/21.
//

#include "device.h"
#include "cerradura_device.h"
#include "cam.h"
#include "comm_dev.h"
#include "esp_log.h"

static const char *TAG = "DEVICE";

int device_init(device_t *d, device_tipo_t c)
{
    switch (c)
    {
        case D_CAMARA:
            d->tipo = D_CAMARA;
            dev_camera_t *camara = malloc(sizeof(dev_camera_t));
            d->context = camara;
            d->init = camera_device_init;
            break;
        case D_PUERTA:
            d->tipo = D_PUERTA;
            dev_cerradura_t *cerradura = malloc(sizeof(dev_cerradura_t));
            d->context = cerradura;
            d->init = cerradura_device_init;
            break;
        case D_COMM:
            d->tipo = D_COMM;
            dev_comm_t *comm = malloc(sizeof(dev_comm_t));
            d->context = comm;
            d->init = comm_device_init;
            break;
        default:
            ESP_LOGE(TAG,"Error en tipo de dispositivo");
    }
    d->init(d);

    return 0;
}
