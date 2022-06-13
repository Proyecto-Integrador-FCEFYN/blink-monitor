//
// Created by agustin on 25/05/22.
//

#ifndef BLINK_COMM_DEV_H
#define BLINK_COMM_DEV_H
#include "mqtt_client.h"
#include "cam.h"
#include "rfid_handler.h"

typedef struct
{
    esp_mqtt_client_handle_t *client;
    dev_camera_t *cam_device;
    rfid_handler_t *rfid_device;
    char *debug_msj;
} dev_comm_t;



void comm_device_init(dev_comm_t *self, dev_camera_t *cam_device, rfid_handler_t *rfid_device);
void comm_enviarfoto(dev_comm_t *self);
void comm_rechazaringreso(dev_comm_t *self);
void comm_aceptaringreso(dev_comm_t *self);
void comm_comunicartimeout(dev_comm_t *self);
void comm_debug_msg(dev_comm_t *self);
void comm_enviarcodigo(dev_comm_t *self);


#endif //BLINK_COMM_DEV_H
