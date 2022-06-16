//
// Created by agustin on 25/05/22.
//

#include <mqtt_client.h>
#include <esp_log.h>
#include "comm_dev.h"
#include "macros.h"
#include "cam.h"

static const char *TAG = "COMM";

static esp_mqtt_client_handle_t my_client;
static dev_camera_t *my_camera;

void comm_device_init(dev_comm_t *self, esp_mqtt_client_handle_t *client, dev_camera_t *camera_device, rfid_handler_t *rfid_device )
{
    my_camera = camera_device;
//        self->client = client;
    self->cam_device = camera_device;
    self->rfid_device = rfid_device;
     ESP_LOGI(TAG, "Cliente MQTT inicializado!");

    esp_mqtt_client_config_t mqtt_cfg = {
            .host = MY_MQTT_HOST,
            .port = MY_MQTT_PORT,
//            .username = MY_MQTT_USER,
//            .password = MY_MQTT_PASS,
            .user_context = self,
            .client_id = "MQTT_DEV"
    };
    my_client = esp_mqtt_client_init(&mqtt_cfg);

    esp_mqtt_client_start(my_client);

}

static int send_msg(dev_comm_t *self, char *data, char *topic)
{
    ESP_ERROR_CHECK( esp_mqtt_client_publish(my_client,
                                             topic,
                                             data,
                                             strlen(data),
                                             0,
                                             0) );
    return 0;


}

void comm_enviarfoto(dev_comm_t *self)
{
//    const char buf;
//    int size;
//    memcpy((void*)&size, &self->cam_device->pic->len, sizeof (int));
//    memcpy( (void*) &buf, self->cam_device->pic->buf,self->cam_device->pic->len);
//    mem
//    int len;
//    esp_mqtt_client_handle_t client = *self->client;
//    camera_fb_t *pic = my_camera->pic;
//    char *saludo = "HOLA!";

//    printf("LALALALALLAA %i\n",pic0->len);
    esp_mqtt_client_publish(my_client,
                            HOST"/"CAMARA_TOPIC,
                            (const char *) pic0->buf,
                            (int) pic0->len,
                            0,
                            0);
}
void comm_rechazaringreso(dev_comm_t *self)
{
    send_msg(self,"Se rechazo ingreso",HOST"/"PUERTA_TOPIC);
}

void comm_enviarcodigo(dev_comm_t *self)
{
    esp_mqtt_client_publish(my_client,
                            HOST"/"RFID_TOPIC,
                            RFIDcurrentState,
                            14,
                            0,
                            0);
//    send_msg(self,RFIDcurrentState,HOST"/"RFID_TOPIC);
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
    ESP_ERROR_CHECK( esp_mqtt_client_publish(my_client,
                                             HOST"/"DEBUG_TOPIC,
            self->debug_msj,
            strlen(self->debug_msj),
            0,
            0) );
}
