/* Blink Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "monitor.h"
#include "procesador_petri.h"
#include "devices/device.h"

#define _REENTRANT
#define _POSIX_PRIORITY_SCHEDULING

void app_main(void)
{
    monitor_t monitor;
    procesador_petri_t petri;
    procesador_de_petri_init(&petri);
    monitor_init(&monitor, &petri);

    //ACA EL ORDEN IMPORTA POR COMO SE ACOMODA EL HEAP.
    //DEBE SER EL PRIMER DISPOSITIVO EN INICIAR!


    device_t mqtt_device;
    device_init(&mqtt_device, &monitor, CONSULTA_MQTT);
    device_t cam_device;
    device_init(&cam_device, &monitor,CAMARA);
//ACA ESTA EL PROBLEMA
//  device_t boton_device;
//  device_init(&boton_device, &monitor, BOTON);
// HASTA ACA
  device_t cerradura_device;
  device_init(&cerradura_device, &monitor, PUERTA);

//  Muy importante que esta función no muera.
    while (1)
    {
        vTaskDelay(10);
    }
}

