//
// Created by agustin on 21/10/21.
// Aca vamos a centralizar las constantes a lo largo del proyecto
// e incluirlas en los archivos que las necesiten
//

#ifndef PTHREAD_MONITOR_IA32_MACROS_H
#define PTHREAD_MONITOR_IA32_MACROS_H

#define DEBUG 1
#define LOG 0

//RED DE PETRI

#define PLAZAS 10
#define TRANSICIONES 11

#define MARCADO \
0,0,0,0,1,0,1,1,0,0

#define INCIDENCIA \
1,-1,0,0,0,0,0,0,0,0,0 ,\
0,1,-1,0,0,-1,0,0,0,0,0,\
0,0,1,-1,0,0,1,0,0,0,0 ,\
0,0,0,1,-1,0,0,0,0,0,0 ,\
-1,0,0,0,1,1,0,-1,0,0,1,\
0,0,0,0,0,0,-1,0,0,1,-1,\
-1,0,1,0,0,1,0,0,0,0,0 ,\
0,0,0,0,0,0,1,0,-1,0,1 ,\
0,0,0,0,0,0,0,0,1,-1,0 ,\
0,0,0,0,0,0,0,1,-1,0,0

// CONFIGURACION WIFI
// POR AHORA ESTAN EN EL MENUCONFIG

// CONFIGURACION MQTT
#define MY_MQTT_HOST "24.232.22.21"
#define MY_MQTT_PORT 27015
#define MY_MQTT_USER "agus"
#define MY_MQTT_PASS "tin"

// CONFIGURACION BOTON DEVICE
//#define MY_GPIO_BOTON
//#define MY_DEBOUNCE_TIME

#endif //PTHREAD_MONITOR_IA32_MACROS_H
