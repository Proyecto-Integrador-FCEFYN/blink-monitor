//
// Created by agustin on 04/07/22.
//

#ifndef BLINK_STREAMING_H
#define BLINK_STREAMING_H

#include <esp_http_server.h>

httpd_handle_t start_webserver(void);

void stop_webserver(httpd_handle_t server);

void connect_handler(void* arg, esp_event_base_t event_base,
                            int32_t event_id, void* event_data);

void disconnect_handler(void* arg, esp_event_base_t event_base,
                               int32_t event_id, void* event_data);
#endif //BLINK_STREAMING_H
