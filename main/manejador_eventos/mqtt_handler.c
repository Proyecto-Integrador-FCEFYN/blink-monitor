//
// Created by agustin on 21/11/21.
//
#include "freertos/FreeRTOS.h"
#include <esp_err.h>
#include <mqtt_client.h>
#include <esp_log.h>
#include <protocol_examples_common.h>
#include "macros.h"
#include "mqtt_handler.h"

static const char *TAG = "MQTT_HANDLER";

static void log_error_if_nonzero(const char * message, int error_code)
{
    if (error_code != 0) {
        ESP_LOGE(TAG, "Last error %s: 0x%x", message, error_code);
    }
}

esp_err_t mqtt_event_handler_cb(esp_mqtt_event_handle_t event)
{
    mqtt_handler_t *handler = event->user_context;
    if(handler->enabled)
    {

        esp_mqtt_client_handle_t client = event->client;
        int msg_id;
        switch (event->event_id) {
            case MQTT_EVENT_CONNECTED:
                ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
                msg_id = esp_mqtt_client_subscribe(client, "topic/confirm", 0);
                ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);
                break;

            case MQTT_EVENT_DISCONNECTED:
                ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
                break;

            case MQTT_EVENT_SUBSCRIBED:
                ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
                msg_id = esp_mqtt_client_publish(client, "/topic/qos0", "data", 0, 0, 0);
                ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
                break;

            case MQTT_EVENT_UNSUBSCRIBED:
                ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
                break;
            case MQTT_EVENT_PUBLISHED:
                ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
                break;
            case MQTT_EVENT_DATA:

                ESP_LOGI(TAG, "MQTT_EVENT_DATA");
                printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
                printf("DATA=%.*s\r\n", event->data_len, event->data);

                if (strncmp(HOST"/"CONFIRM_TOPIC, event->topic, event->topic_len ) == 0 )
                {
                    if (strncmp("denegado_boton", event->data, event->data_len)== 0)
                    {
                        handler->monitor->disparar(handler->monitor, T_DENEGADO_BOTON);
                        ESP_LOGI(TAG,"EL SISTEMA HA DENEGADO EL ACCESO POR BOTON");
                    }
                    else if (strncmp("permitido_boton", event->data, event->data_len) == 0)
                    {
                        handler->monitor->disparar(handler->monitor, T_PERMITIDO_BOTON);
                        ESP_LOGI(TAG,"EL SISTEMA HA PERMITIDO EL ACCESO POR BOTON");
                    }
                    else if (strncmp("denegado_rfid", event->data, event->data_len) == 0)
                    {
                        handler->monitor->disparar(handler->monitor, T_DENEGADO_RFID);
                        ESP_LOGI(TAG,"EL SISTEMA HA DENEGADO EL ACCESO POR RFID");
                    }
                    else if (strncmp("permitido_rfid", event->data, event->data_len) == 0)
                    {
                        handler->monitor->disparar(handler->monitor, T_PERMITIDO_RFID);
                        ESP_LOGI(TAG,"EL SISTEMA HA PERMITIDO EL ACCESO POR RFID");
                    }
                    else
                        ESP_LOGE(TAG,"EL MENSAJE RECIBIDO ES ERRONEO");
                }
                break;
            case MQTT_EVENT_ERROR:
                ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
                if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT) {
                    log_error_if_nonzero("reported from esp-tls", event->error_handle->esp_tls_last_esp_err);
                    log_error_if_nonzero("reported from tls stack", event->error_handle->esp_tls_stack_err);
                    log_error_if_nonzero("captured as transport's socket errno",  event->error_handle->esp_transport_sock_errno);
                    ESP_LOGI(TAG, "Last errno string (%s)", strerror(event->error_handle->esp_transport_sock_errno));

                }
                break;
            default:
                ESP_LOGI(TAG, "Other event id:%d", event->event_id);
                break;
        }
    }
    return ESP_OK;
}

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
    LWIP_UNUSED_ARG(handler_args);
    LWIP_UNUSED_ARG(base);
    LWIP_UNUSED_ARG(event_id);
    mqtt_event_handler_cb(event_data);

}


void mqtt_handler_init(mqtt_handler_t *self, monitor_t *monitor)
{
    esp_mqtt_client_config_t mqtt_cfg = {
            .host = MY_MQTT_HOST,
            .port = MY_MQTT_PORT,
//            .username = MY_MQTT_USER,
//            .password = MY_MQTT_PASS,
            .user_context = self
    };
    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client,
                                   ESP_EVENT_ANY_ID,
                                   mqtt_event_handler,
                                   NULL);
    esp_mqtt_client_start(client);

    self->monitor = monitor;
    self->enabled = 0;
    self->buffer = "";
    self->buffer_size = 0;
}