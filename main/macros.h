//
// Created by agustin on 21/10/21.
// Aca vamos a centralizar las constantes a lo largo del proyecto
// e incluirlas en los archivos que las necesiten
//

#ifndef PTHREAD_MONITOR_IA32_MACROS_H
#define PTHREAD_MONITOR_IA32_MACROS_H

#define PLAZAS 10
#define TRANSICIONES 11
#define DEBUG 1
#define LOG 0

//MATRICES

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

#define MARCADO \
0,0,0,0,1,0,1,1,0,0
//1,0,0,1,0,0,0,3

#endif //PTHREAD_MONITOR_IA32_MACROS_H
/*-1,0,0,0,1,0, \*/
/*1,0,0,-1,0,0, \*/
/*0,0,0,1,-1,0, \*/
/*0,-1,0,0,0,1, \*/
/*0,1,-1,0,0,0, \*/
/*0,0,1,0,0,-1, \*/
/*1,-1,0,0,0,0, \*/
/*-1,1,0,0,0,0   */