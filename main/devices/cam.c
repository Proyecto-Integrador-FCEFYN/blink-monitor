//
// Created by agustin on 16/11/21.
//

#include "cam.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <esp_log.h>
#include <esp_system.h>
#include <nvs_flash.h>
#include <sys/param.h>

#include "esp_camera.h"

static const char *TAG = "CAMARA";

void camera_sacarfoto(dev_camera_t *self)
{
//    esp_camera_fb_return(self->pic);
    esp_camera_fb_return(pic0);

    sleep(1);
//    self->pic = esp_camera_fb_get();
    pic0 = esp_camera_fb_get();
    ESP_LOGI(TAG, "Picture taken! Its size was: %i bytes", pic0->len);
}

void camera_device_init(dev_camera_t *self)
{
    static camera_config_t camera_config = {
            .pin_pwdn = CAM_PIN_PWDN,
            .pin_reset = CAM_PIN_RESET,
            .pin_xclk = CAM_PIN_XCLK,
            .pin_sscb_sda = CAM_PIN_SIOD,
            .pin_sscb_scl = CAM_PIN_SIOC,

            .pin_d7 = CAM_PIN_D7,
            .pin_d6 = CAM_PIN_D6,
            .pin_d5 = CAM_PIN_D5,
            .pin_d4 = CAM_PIN_D4,
            .pin_d3 = CAM_PIN_D3,
            .pin_d2 = CAM_PIN_D2,
            .pin_d1 = CAM_PIN_D1,
            .pin_d0 = CAM_PIN_D0,
            .pin_vsync = CAM_PIN_VSYNC,
            .pin_href = CAM_PIN_HREF,
            .pin_pclk = CAM_PIN_PCLK,

            //XCLK 20MHz or 10MHz for OV2640 double FPS (Experimental)
            .xclk_freq_hz = 20000000,
            .ledc_timer = LEDC_TIMER_0,
            .ledc_channel = LEDC_CHANNEL_0,

//            .pixel_format = PIXFORMAT_RGB565, //YUV422,GRAYSCALE,RGB565,JPEG
            .pixel_format = PIXFORMAT_JPEG, //YUV422,GRAYSCALE,RGB565,JPEG
            .frame_size = FRAMESIZE_QVGA,    //QQVGA-UXGA Do not use sizes above QVGA when not JPEG

            .jpeg_quality = 12, //0-63 lower number means higher quality
            .fb_count = 1,       //if more than one, i2s runs in continuous mode. Use only with JPEG
            .grab_mode = CAMERA_GRAB_WHEN_EMPTY,
    };
    //initialize the camera
    esp_err_t err = esp_camera_init(&camera_config);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Camera Init Failed");
    }
}



