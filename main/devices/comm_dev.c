//
// Created by agustin on 25/05/22.
//

#include <mqtt_client.h>
#include <esp_log.h>
#include "comm_dev.h"
#include "macros.h"
#include "device.h"
#include "cam.h"

static const char *TAG = "COMM";

void comm_device_init(device_t *self)
{
    esp_mqtt_client_config_t mqtt_cfg = {
            .host = MY_MQTT_HOST,
            .port = MY_MQTT_PORT,
//            .username = MY_MQTT_USER,
//            .password = MY_MQTT_PASS,
    };

    dev_comm_t  *this_comm_device = self->context;
    if ( (*this_comm_device->client = esp_mqtt_client_init(&mqtt_cfg)) )
    {
        ESP_LOGE(TAG, "Error en la inicializacion del cliente MQTT");
    }
    ESP_ERROR_CHECK(esp_mqtt_client_start(*this_comm_device->client));
}

static int send_msg(dev_comm_t *self, char *data, char *topic)
{
    ESP_ERROR_CHECK( esp_mqtt_client_publish(*self->client,
                                             topic,
                                             data,
                                             strlen(data),
                                             0,
                                             0) );
    return 0;
}

void comm_enviarfoto(device_t *self, device_t *cam_device)
{
    dev_comm_t  *this_comm_device = self->context;
    dev_camera_t *this_camera_device = cam_device->context;

    esp_mqtt_client_publish(*this_comm_device->client,
                            HOST"/"CAMARA_TOPIC,
                            (char*) this_camera_device->pic->buf,
                            (int) this_camera_device->pic->len,
                            0,
                            0);
}
void comm_rechazaringreso(device_t *self)
{
    dev_comm_t *this_comm_device = self->context;
    send_msg(this_comm_device,"Se rechazo ingreso",HOST"/"PUERTA_TOPIC);
}
void comm_comunicartimeout(device_t *self)
{
    dev_comm_t *this_comm_device = self->context;
    send_msg(this_comm_device,"Timeout en cerradura",HOST"/"PUERTA_TOPIC);
}

void comm_debug_msg(dev_comm_t *self, char *data)
{
    ESP_ERROR_CHECK( esp_mqtt_client_publish(*self->client,
                                             HOST"/"DEBUG_TOPIC,
                                             data,
                                             strlen(data),
                                             0,
                                             0) );
}
