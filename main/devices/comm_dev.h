//
// Created by agustin on 25/05/22.
//

#ifndef BLINK_COMM_DEV_H
#define BLINK_COMM_DEV_H
#include "mqtt_client.h"
#include "device.h"

typedef struct
{
    esp_mqtt_client_handle_t *client;
} dev_comm_t;



void comm_device_init(device_t *self);
void comm_enviarfoto(device_t *self, device_t *camara_device);
void comm_rechazaringreso(device_t *self);
void comm_comunicartimeout(device_t *self);
void comm_debug_msg(dev_comm_t *self, char *data);

#endif //BLINK_COMM_DEV_H
