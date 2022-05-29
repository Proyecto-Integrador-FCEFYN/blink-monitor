//
// Created by agustin on 28/04/22.
//


#ifndef BLINK_RFID_HANDLER_H
#define BLINK_RFID_HANDLER_H

#include "handler.h"

void rfid_handler_init(handler_t *h);

_Noreturn void* rfid_handler_task(void* arg);

#endif //BLINK_RFID_HANDLER_H
