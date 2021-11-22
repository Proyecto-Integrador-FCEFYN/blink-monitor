//
// Created by agustin on 16/11/21.
//

#include "mqtt_device.h"

#include <stdio.h>
#include <stdint.h>
//#include <stddef.h>
#include <string.h>
#include <monitor.h>
//#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "protocol_examples_common.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
//#include "freertos/semphr.h"
//#include "freertos/queue.h"
//
//#include "lwip/sockets.h"
//#include "lwip/dns.h"
//#include "lwip/netdb.h"

#include "esp_log.h"
#include "mqtt_client.h"
#include "device.h"
#include "cam.h"
#include "macros.h"

static const char *TAG = "MQTT_EXAMPLE";

static esp_mqtt_client_handle_t client;
static device_t *device;
static int bandera = 0;

_Noreturn void* mqtt_device_task(void* arg)
{
    while (1)
    {
        device_t *self = arg;
        if(self->enabled)
        {
            self->monitor->disparar(self->monitor, 9);
            int msg_id = esp_mqtt_client_publish(client, TOPIC_FOTO, (char*) pic->buf, (int) pic->len, 0, 0);
            ESP_LOGI(TAG, "Agustin probando, ID: %i" , msg_id);


        }
        vTaskDelay(10);
    }
}

static void log_error_if_nonzero(const char * message, int error_code)
{
    if (error_code != 0) {
        ESP_LOGE(TAG, "Last error %s: 0x%x", message, error_code);
    }
}

static esp_err_t mqtt_event_handler_cb(esp_mqtt_event_handle_t event)
{
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;
    // your_context_t *context = event->context;
    switch (event->event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
//            msg_id = esp_mqtt_client_publish(client, "/topic/qos1", "data_3", 0, 1, 0);
//            ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);

//            msg_id = esp_mqtt_client_subscribe(client, "/topic/qos0", 0);
//            ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

            msg_id = esp_mqtt_client_subscribe(client, "/topic/confirm", 0);
            ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

//            msg_id = esp_mqtt_client_subscribe(client, "/topic/qos1", 1);
//            ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);
//
//            msg_id = esp_mqtt_client_unsubscribe(client, "/topic/qos1");
//            ESP_LOGI(TAG, "sent unsubscribe successful, msg_id=%d", msg_id);
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
            char* topico = "/topic/confirm";
            int i = strncmp(topico, event->topic, strlen(topico) );
            printf("EL VALOR DE i es %i\n", i);

            if (i == 0 )
            {
                char *msg_no = "rechazado";
                char *msg_si = "aceptado";
                if (strncmp(msg_no, event->data, strlen(msg_no))== 0)
                {
                    ESP_LOGI(TAG,"EL SISTEMA HA DENEGADO EL ACCESO");
                    device->monitor->disparar(device->monitor, 10);
                }
                else if (strncmp(msg_si, event->data, strlen(msg_si)) == 0)
                {
                    ESP_LOGI(TAG,"EL SISTEMA HA PERMITIDO EL ACCESO");
                    device->monitor->disparar(device->monitor, 6);
                }
                else
                    ESP_LOGI(TAG,"EL MENSAJE RECIBIDO ES ERRONEO");
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
    return ESP_OK;
}

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%d", base, event_id);

    mqtt_event_handler_cb(event_data);
}

void mqtt_device_init(device_t *d, monitor_t *m)
{
    ESP_LOGI(TAG, "[APP] Startup..");
    ESP_LOGI(TAG, "[APP] Free memory: %d bytes", esp_get_free_heap_size());
    ESP_LOGI(TAG, "[APP] IDF version: %s", esp_get_idf_version());

    esp_log_level_set("*", ESP_LOG_INFO);
    esp_log_level_set("MQTT_CLIENT", ESP_LOG_VERBOSE);
    esp_log_level_set("MQTT_EXAMPLE", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT_TCP", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT_SSL", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT", ESP_LOG_VERBOSE);
    esp_log_level_set("OUTBOX", ESP_LOG_VERBOSE);

    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    /* This helper function configures Wi-Fi or Ethernet, as selected in menuconfig.
     * Read "Establishing Wi-Fi or Ethernet Connection" section in
     * examples/protocols/README.md for more information about this function.
     */
    ESP_ERROR_CHECK(example_connect());

    esp_mqtt_client_config_t mqtt_cfg = {
            .host = MY_MQTT_HOST,
            .port = MY_MQTT_PORT,
            .username = MY_MQTT_USER,
            .password = MY_MQTT_PASS,
    };
    device = d;

//    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
    client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
    esp_mqtt_client_start(client);
}

