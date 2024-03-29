//
// Created by agustin on 28/04/22.
//


#ifndef BLINK_RFID_HANDLER_H
#define BLINK_RFID_HANDLER_H

#include "monitor.h"

typedef struct {
    monitor_t *monitor;
    char *buffer;
    unsigned int buffer_size;
    int enabled;
} rfid_handler_t;

void rfid_handler_init(rfid_handler_t *h, monitor_t *m);

//void guardar_rfid(handler_t *self);

char RFIDcurrentState[14];
int current_tag;

void enviar_evento_rfid(); 

_Noreturn void* rfid_handler_task(void* arg);
_Noreturn void* v2_rfid_handler_task(void* arg);

#endif //BLINK_RFID_HANDLER_H
