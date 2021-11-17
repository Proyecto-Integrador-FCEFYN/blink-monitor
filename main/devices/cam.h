//
// Created by agustin on 16/11/21.
//

#ifndef BLINK_CAM_H
#define BLINK_CAM_H

#include <esp_camera.h>
#include <monitor.h>
#include "device.h"

#define BOARD_ESP32CAM_AITHINKER

// ESP32Cam (AiThinker) PIN Map
#ifdef BOARD_ESP32CAM_AITHINKER
#define CAM_PIN_PWDN 32
#define CAM_PIN_RESET -1 //software reset will be performed
#define CAM_PIN_XCLK 0
#define CAM_PIN_SIOD 26
#define CAM_PIN_SIOC 27
#define CAM_PIN_D7 35
#define CAM_PIN_D6 34
#define CAM_PIN_D5 39
#define CAM_PIN_D4 36
#define CAM_PIN_D3 21
#define CAM_PIN_D2 19
#define CAM_PIN_D1 18
#define CAM_PIN_D0 5
#define CAM_PIN_VSYNC 25
#define CAM_PIN_HREF 23
#define CAM_PIN_PCLK 22
#endif //BOARD_ESP32CAM_AITHINKER

camera_fb_t *pic;

void init_camera(device_t *d, monitor_t *m);

_Noreturn void* camera_task(void* arg);

#endif //BLINK_CAM_H
