//
// Created by agustin on 8/11/21.
//

#include "boton_device.h"

#include <sys/cdefs.h>
#include "driver/gpio.h"
#include <stdio.h>

#define GPIO_INPUT_IO_0     16  // TODO: ELEGIR PIN PARA BOTON
#define GPIO_INPUT_PIN_SEL  ((1ULL<<GPIO_INPUT_IO_0) )
#define ESP_INTR_FLAG_DEFAULT 0

//static xQueueHandle gpio_evt_queue = NULL;

static void IRAM_ATTR gpio_isr_handler_boton(void* arg)
{
    printf("Entre al handler de boton\n");

    uint32_t gpio_num = (uint32_t) arg;
//    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

_Noreturn void* gpio_task_boton(void* arg)
{
    printf("entre a la task del boton!\n");
    // T7, T8, T9
//    uint32_t io_num;
//    device_t *self = arg;
    while(1) {
        //if(!gpio_get_level(GPIO_INPUT_IO_0)) {
            printf("lei un valor en el pin!\n");
        sleep(5);

        printf("Lei: %i\n",gpio_get_level(16));


            ////        if(xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) {
//            self->monitor->disparar(self->monitor, 7);
//            printf("Disparar 7");
//            sleep(1);
//            self->monitor->disparar(self->monitor, 8);
//            printf("Disparar 8");
//            sleep(1);
//            self->monitor->disparar(self->monitor, 9);
//            printf("Disparar 9");
            //sleep(1);
       // }
    }
}

void gpio_device_init(device_t *d, monitor_t *m)
{

    gpio_config_t io_conf = {}; //zero-initialize the config structure.

    // GPIO INPUT
    io_conf.intr_type = GPIO_INTR_POSEDGE; //interrupt of rising edge
    io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL; //bit mask of the pins, use GPIO4/5 here
    io_conf.mode = GPIO_MODE_INPUT; //set as input mode
    io_conf.pull_up_en = 1; //enable pull-up mode
    io_conf.pull_down_en = 0; //disable pull-down mode
    gpio_config(&io_conf);

//    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t)); //create a queue to handle gpio event from isr

//    xTaskCreate(gpio_task_boton, "gpio_task_boton", 2048, NULL, 10, NULL); //start gpio task
    //gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT); //install gpio isr service

    //gpio_isr_handler_add(GPIO_INPUT_IO_0, gpio_isr_handler_boton, (void *) GPIO_INPUT_IO_0); //hook isr handler for specific gpio pin
    printf("Inicialice correctamente el boton\n");
}
