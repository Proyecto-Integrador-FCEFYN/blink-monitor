//
// Created by agustin on 21/10/21.
// Aca vamos a centralizar las constantes a lo largo del proyecto
// e incluirlas en los archivos que las necesiten
// DESPUES SE PUEDEN PONER EN EL MENUCONFIG!
//

#ifndef PTHREAD_MONITOR_IA32_MACROS_H
#define PTHREAD_MONITOR_IA32_MACROS_H

#define DEBUG 1
#define LOG 0

// CONFIGURACION TOPICS MQTT
#define HOST "lac1"
// RECURSOS
#define PUERTA_TOPIC "puerta"
#define RFID_TOPIC "rfid"
#define CAMARA_TOPIC "camara"
#define CONFIRM_TOPIC "confirm"
#define DEBUG_TOPIC "debug"


//CONFIGURACION RED DE PETRI

#define PLAZAS 14
#define TRANSICIONES 14
#define HILOS 6

#define MARCADO \
0,0,0,0,0,1,1,1,1,5,0,0,0,1


#define INCIDENCIA \
1,0,0,0,0,0,-1,0,0,0,0,0,0,0, \
0,1,0,0,0,0,0,0,-1,0,0,0,0,0, \
0,0,1,0,0,0,0,0,0,-1,0,0,0,0, \
0,0,0,1,0,0,0,0,0,0,-1,0,0,0, \
0,0,0,0,1,0,0,0,0,0,0,-1,0,0, \
0,0,0,-1,0,0,0,0,0,0,1,0,0,0, \
-1,0,0,0,0,0,1,0,0,0,0,0,0,0, \
0,-1,-1,0,0,0,0,1,0,1,0,0,0,0, \
0,0,0,0,-1,0,0,0,0,0,0,0,0,1, \
-1,-1,-1,-1,-1,-1,1,1,0,1,1,0,1,1, \
0,0,0,0,0,0,0,-1,1,0,0,0,0,0, \
0,0,0,0,0,0,0,0,0,0,0,1,0,-1, \
0,0,0,0,0,1,0,0,0,0,0,0,-1,0, \
0,0,0,0,0,-1,0,0,0,0,0,0,1,0

// CONFIGURACION WIFI
// POR AHORA ESTAN EN EL MENUCONFIG

// CONFIGURACION MQTT
//#define MY_MQTT_HOST "24.232.22.21"
#define MY_MQTT_HOST "raspi"
//#define MY_MQTT_PORT 27015
#define MY_MQTT_PORT 1883
#define MY_MQTT_USER "agus"
#define MY_MQTT_PASS "tin"

// CONFIGURACION BOTON DEVICE
#define BOTON_GPIO 2
#define BOTON_DEBOUNCE_TIME 50

// CONFIGURACION MOVIMIENTO DEVICE
#define MOVIMIENTO_GPIO 12
#define MOVIMIENTO_DEBOUNCE_TIME 50


// CONFIGURACION RFID
#define RFID_RX_PORT 13

// CONFIGURACION CERRADURA
#define CERRADURA_GPIO 14
#define CERRADURA_TIMEOUT 10 //segundos
#define CERRADURA_ABIERTA 5

// CONFIGURACION BUZZER
#define BUZZER_GPIO 14

// FIXME: Hay que rehacer esto. Se actualizo la RDP.
// 
// EVENTOS DE LAS TRANSICIONES
// HANDLER MQTT T
#define T_PERMITIDO_BOTON 6
#define T_DENEGADO_BOTON 10
#define T_PERMITIDO_RFID 2
#define T_DENEGADO_RFID 5
// HANDLER RFID T
#define T_EVENTO_RFID 0
// HANDLER BOTON T
#define T_EVENTO_BOTON 7

// NUEVAS TRANSICIONES
#define T_BOTON_SOLICITUD 0
#define T_RFID_DETECCION_NUEVO 1
#define T_RFID_SOLICITUD_ACTUAL 2
#define T_CERRADURA_SOLICITUD 3
#define T_FOTO_SOLICITUD 4
#define T_MOVIMIENTO_DETECCION 5
#define T_BOTON_ENVIAR 6
#define T_RFID_ENVIAR_NUEVO 7
#define T_RFID_ACTUALIZAR_NUEVO 8
#define T_RFID_ENVIAR_ACTUAL 9
#define T_CERRADURA_ENVIAR 10
#define T_FOTO_SACAR 11
#define T_MOVIMIENTO_ENVIAR 12
#define T_FOTO_ENVIAR 13


// HTTP
#define API_BASE_URL "https://192.168.1.110"
#define API_USERNAME "usuario-api"
#define API_PASSWORD "password-api"

#define HTTP_AUTH_USERNAME "usuario-esp32"
#define HTTP_AUTH_PASSWORD "password-esp32"


#define NULL_TRANSITION -1
#endif //PTHREAD_MONITOR_IA32_MACROS_H
