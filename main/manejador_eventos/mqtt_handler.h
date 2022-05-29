//
// Created by agustin on 21/11/21.
//

#ifndef BLINK_MQTT_HANDLER_H
#define BLINK_MQTT_HANDLER_H

void mqtt_handler_init(handler_t *h);

_Noreturn void *mqtt_handler_task(void* arg);

#endif //BLINK_MQTT_HANDLER_H
