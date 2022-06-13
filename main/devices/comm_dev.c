//
// Created by agustin on 25/05/22.
//

#include <mqtt_client.h>
#include <esp_log.h>
#include "comm_dev.h"
#include "macros.h"
#include "cam.h"

static const char *TAG = "COMM";

void comm_device_init(dev_comm_t *self, dev_camera_t *cam_device, rfid_handler_t *rfid_device )
{
    self->cam_device = cam_device;
    self->rfid_device = rfid_device;

    esp_mqtt_client_config_t mqtt_cfg = {
            .host = MY_MQTT_HOST,
            .port = MY_MQTT_PORT,
//            .username = MY_MQTT_USER,
//            .password = MY_MQTT_PASS,
            .user_context = self
    };

//    dev_comm_t  self = self->context;
//    *self->client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);

    ESP_ERROR_CHECK(esp_mqtt_client_start(client));
    self->client = client;
}

static int send_msg(dev_comm_t *self, char *data, char *topic)
{
    ESP_ERROR_CHECK( esp_mqtt_client_publish(self->client,
                                             topic,
                                             data,
                                             strlen(data),
                                             0,
                                             0) );
    return 0;


}

void comm_enviarfoto(dev_comm_t *self)
{

    esp_mqtt_client_publish(self->client,
                            HOST"/"CAMARA_TOPIC,
                            (char*) self->cam_device->pic->buf,
                            (int) self->cam_device->pic->len,
                            0,
                            0);
}
void comm_rechazaringreso(dev_comm_t *self)
{
    send_msg(self,"Se rechazo ingreso",HOST"/"PUERTA_TOPIC);
}

void comm_enviarcodigo(dev_comm_t *self)
{
    send_msg(self,self->rfid_device->buffer,HOST"/"RFID_TOPIC);
}

void comm_aceptaringreso(dev_comm_t *self)
{
    send_msg(self,"Se acepto ingreso",HOST"/"PUERTA_TOPIC);
}
void comm_comunicartimeout(dev_comm_t *self)
{
    send_msg(self,"Timeout en cerradura",HOST"/"PUERTA_TOPIC);
}

void comm_debug_msg(dev_comm_t *self)
{
    ESP_ERROR_CHECK( esp_mqtt_client_publish(self->client,
                                             HOST"/"DEBUG_TOPIC,
            self->debug_msj,
            strlen(self->debug_msj),
            0,
            0) );
}
