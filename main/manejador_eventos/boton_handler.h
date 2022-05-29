//
// Created by agustin on 8/11/21.
//
#ifndef CLASE_GPIO_HANDLER_H
#define CLASE_GPIO_HANDLER_H

#include <sys/cdefs.h>
#include "handler.h"

void boton_handler_init(handler_t *h);

_Noreturn void* boton_handler_task(void* arg);

#endif /* CLASE_GPIO_HANDLER_H */
