/* Blink Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <protocol_examples_common.h>
#include <esp_log.h>
#include <nvs_flash.h>
#include <esp_event.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "monitor.h"
#include "procesador_petri.h"
#include "devices/device.h"
#include "software/software.h"
#include "devices/comm_dev.h"
#include "devices/cerradura_device.h"

#define _REENTRANT
#define _POSIX_PRIORITY_SCHEDULING

void app_main(void) {
    monitor_t monitor;
    procesador_petri_t petri;
    procesador_de_petri_init(&petri);
    monitor_init(&monitor, &petri);

    //ACA EL ORDEN IMPORTA POR COMO SE ACOMODA EL HEAP.
    //DEBE SER EL PRIMER DISPOSITIVO EN INICIAR!

    //TODO: ARMAR INICIALIZACION DE DISPOSITIVOS
    //TODO: ARMAR INICIALIZACION DE HANDLERS
    //TODO: ARMAR INICIALIZACION DE SOFTWARE
    //TODO: ARMAR INICIALIZACION DE SECUENCIAS DE DISPAROS


//    device_t mqtt_device;
//    device_init(&mqtt_device, &monitor, D_COMM);
//    device_t cam_device;
//    device_init(&cam_device, &monitor, CAMARA);
//    device_t boton_device;
//    device_init(&boton_device, &monitor, BOTON);
//    device_t cerradura_device;
//    device_init(&cerradura_device, &monitor, PUERTA);
//    device_t rfid_device;
//    device_init(&rfid_device, &monitor, RFID);

    // ENABLE
//    device_enable(&cam_device);
//    device_enable(&mqtt_device);
//    device_enable(&boton_device);
//    device_enable(&cerradura_device);
//    device_enable(&rfid_device);

    // SOFTWARE
    seqdisparo_t seq1,seq2;
    int seq1_transiciones[3] = {8, 9, 10};
    void *seq1_actions[3] = {
            &comm_enviarfoto, //P9
            &comm_rechazaringreso, // P10
            &comm_comunicartimeout, // P11
    };
    seqdisparo_init(&seq1, 4, seq1_transiciones, seq1_actions);

    int seq2_transiciones[2] = {5, 6};
    void *seq2_actions[2] = {
            &comm_enviarfoto,
            &comm_rechazaringreso,
    };
    seqdisparo_init(&seq2, 4, seq2_transiciones, seq2_actions);
    seqdisparo_t seqs[2] = {seq1,seq2};

    software_t software;
    software_init(&software, seqs);

//  Muy importante que esta función no muera.
    while (1)
    {
        vTaskDelay(10);
    }
}

void init_communications()
{
    static const char *TAG = "INIT_COMMUNICATIONS";
    /* This helper function configures Wi-Fi or Ethernet, as selected in menuconfig.
    * Read "Establishing Wi-Fi or Ethernet Connection" section in
    * examples/protocols/README.md for more information about this function.
    */
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

    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
}

