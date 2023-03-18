#include <protocol_examples_common.h>
#include <esp_log.h>
#include <nvs_flash.h>
#include <esp_event.h>
#include <esp_http_server.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "monitor.h"
#include "procesador_petri.h"
#include "software/software.h"
#include "mqtt_client.h"
#include "streaming/streaming.h"
//Handlers
#include "rfid_handler.h"
#include "boton_handler.h"
#include "movimiento_handler.h"
//Devices
#include "cam.h"
#include "comm_dev.h"
#include "cerradura_device.h"

#define _REENTRANT
#define _POSIX_PRIORITY_SCHEDULING

void init_communications()
{
    static const char *TAG = "INIT_COMMUNICATIONS";

    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    ESP_ERROR_CHECK(example_connect());

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
    esp_log_level_set("esp32_asio_pthread", ESP_LOG_VERBOSE);
    esp_log_level_set("*", ESP_LOG_VERBOSE);
}

void app_main(void) {
    init_communications();
    // esp_mqtt_client_handle_t client = NULL;
    monitor_t monitor;
    procesador_petri_t petri;
    procesador_de_petri_init(&petri);
    monitor_init(&monitor, &petri);


    //Init handlers
    boton_handler_t boton_handler;
    boton_handler_init(&boton_handler, &monitor);
    movimiento_handler_t movimiento_handler;
    movimiento_handler_init(&movimiento_handler, &monitor);
    rfid_handler_t rfid_handler;
    rfid_handler_init(&rfid_handler, &monitor);

    // Init devices
    dev_camera_t cam_device;
    camera_device_init(&cam_device);
    dev_cerradura_t cerradura_dev;
    cerradura_device_init(&cerradura_dev,5);

    // SOFTWARE
    segmento_t segmentos[3];

    int seq0[] = {8, 9};
    action_p actions0[] = {
            (action_p)camera_sacarfoto, // P8
            (action_p)comm_enviarfoto   // P5
    };
    objeto_t objetos0[] = {
            (objeto_t)&cam_device,
            NULL
//            (objeto_t)&comm_device

    };

    segmento_init(&segmentos[0],
                  &monitor,
                  seq0,
                  actions0,
                  objetos0,
                  2);

    int seq1[] = {3, 4};
    action_p action1[] = {
            (action_p)cerradura_abrirpuerta, // P2
            NULL
    };
    objeto_t objetos1[] = {
            (objeto_t)&cerradura_dev,
            NULL
    };
    segmento_init(&segmentos[1],
                  &monitor,
                  seq1,
                  action1,
                  objetos1,
                  2);

    int seq2[] = {1};
    action_p actions2[] = {
            (action_p) comm_enviarcodigo
    };
    objeto_t objetos2[] = {
            NULL
//            (objeto_t)&comm_device,
    };
    segmento_init(&segmentos[2],
                  &monitor,
                  seq2,
                  actions2,
                  objetos2,
                  1);

    // ENABLE
    rfid_handler.enabled = 1;
    boton_handler.enabled = 1;

    software_t software;
    software_init(&software, segmentos);


    // *************************** //
    //       Video Streaming       //
    // *************************** //

    // static httpd_handle_t server = NULL;
    start_webserver();

//  Muy importante que esta función no muera.
    while (1)
    {
        vTaskDelay(1000);
    }
}

