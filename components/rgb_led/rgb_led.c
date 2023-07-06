// Copyright (c)IASX Technologies S/A All rights reserved.
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>

#include <driver/gpio.h>


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "esp_log.h"

#include "rgb_led.h"
#include "led_strip.h"


// #ifdef CONFIG_BLINK_LED_RMT

// #else CONFIG_BLINK_LED_GPIO

// #endif


static const char *TAG = "rgb_led";

TaskHandle_t prvblink_color_rgb_led_taskHandle, prvpulse_blink_color_rgb_led_taskHandle;
TaskStatus_t prvblink_color_rgb_led_taskDetails,prvpulse_blink_color_rgb_led_taskDetails;

//bool dev->is_setcolor_ft=true, dev->is_blink_ft=true, dev->is_pulse_blink_ft=true;

rgbled_t* initialize_rgb_led(uint8_t rgbLed_data_pin, uint8_t rgbLed_en_pin)
{
    rgbled_t* dev;

    if ((dev = malloc (sizeof(rgbled_t))) == NULL)
        return NULL;

    // init rgbled data structure
    dev->data_pin = rgbLed_data_pin;
    dev->en_pin   = rgbLed_en_pin;
    // dev->current_led_color = NULL;
    // dev->current_led_mode = NULL;
    dev->is_setcolor_ft=true; 
    dev->is_blink_ft=true;
    dev->is_pulse_blink_ft=true;



    // Configure output rgb_led EN pin
    gpio_config_t io_conf = {
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = 0,
    };
    io_conf.pin_bit_mask = ((uint64_t)1 << dev->en_pin);
    gpio_config(&io_conf); // Configure the GPIO

    // //#TEST: GPIO RGBLED_EN ALWAYS ENABLE
    // gpio_set_level(dev->en_pin, true); 
    // vTaskDelay(100 / portTICK_PERIOD_MS);



    ////LED CONFIG AND INIT
    dev->pStrip = led_strip_init(0, dev->data_pin, 1);  //LED strip Ch 0 initialization with the GPIO "BUILTIN_BOARD_rgb_led_GPIO" and pixels number 1*/
    dev->pStrip->clear(dev->pStrip, 50);   //Set all LED off to clear all pixels
    vTaskDelay(100 / portTICK_PERIOD_MS); // giving a time to device setdown

    //testing all main three colors RGB
    blink_rgb_initilize(dev);




    return dev;
}

static void blink_rgb_initilize(rgbled_t* dev) {
    
    gpio_set_level(dev->en_pin, true);

    vTaskDelay(200 / portTICK_PERIOD_MS); // giving a time to device setdown
    dev->pStrip->set_pixel(dev->pStrip,0,255,0,0);//RED
    dev->pStrip->refresh(dev->pStrip, 100);
    vTaskDelay(200 / portTICK_PERIOD_MS); // giving a time to device setdown
    dev->pStrip->set_pixel(dev->pStrip,0,0,255,0);//GEEN
    dev->pStrip->refresh(dev->pStrip, 100);
    vTaskDelay(200 / portTICK_PERIOD_MS); // giving a time to device setdown
    dev->pStrip->set_pixel(dev->pStrip,0,0,0,255);//BLUE
    dev->pStrip->refresh(dev->pStrip, 100);
    dev->is_setcolor_ft = false;
    vTaskDelay(200 / portTICK_PERIOD_MS); // giving a time to device setdown
    reset_rgb_led(dev);
}

void set_dev_color_mode_rgbled (rgbled_t* dev, led_color_t color, led_mode_t mode) {

    dev->current_led_color = color;
    dev->current_led_mode = mode;

    switch (mode) {
        case led_mode_static:
            set_color_rgb_led(dev, color);
            break;
        case led_mode_blink:
            set_blink_color_rgb_led(dev, color);
            break;
        case led_mode_pulse:
            set_pulse_blink_color_rgb_led(dev, color);
            break;
        default:
            break;
    }
}


static void set_color_rgb_led(rgbled_t* dev, led_color_t color) {

    reset_rgb_led(dev);
    gpio_set_level(dev->en_pin, true);

    switch (color) {
        case led_color_red:
            dev->pStrip->set_pixel(dev->pStrip,0,255,0,0);//RED
            dev->pStrip->refresh(dev->pStrip, 100);
            dev->is_setcolor_ft = false;
            break;
        case led_color_green:
            dev->pStrip->set_pixel(dev->pStrip,0,0,255,0);//GEEN
            dev->pStrip->refresh(dev->pStrip, 100);
            dev->is_setcolor_ft = false;
            break;
        case led_color_blue:
            dev->pStrip->set_pixel(dev->pStrip,0,0,0,255);//BLUE
            dev->pStrip->refresh(dev->pStrip, 100);
            dev->is_setcolor_ft = false;
            break;
        case led_color_yellow:
            dev->pStrip->set_pixel(dev->pStrip,0,255,255,0);//Yellow
            dev->pStrip->refresh(dev->pStrip, 100);
            dev->is_setcolor_ft = false;
            break;
        case led_color_orange:
            dev->pStrip->set_pixel(dev->pStrip,0,255,127,0);//Orange
            dev->pStrip->refresh(dev->pStrip, 100);
            dev->is_setcolor_ft = false;
            break;
        case led_color_purple:
            dev->pStrip->set_pixel(dev->pStrip,0,255,0,255);//purple
            dev->pStrip->refresh(dev->pStrip, 100);
            dev->is_setcolor_ft = false;
            break;
        case led_color_white:
            dev->pStrip->set_pixel(dev->pStrip,0,255,255,255);//white
            dev->pStrip->refresh(dev->pStrip, 100);
            dev->is_setcolor_ft = false;
            break;
        case led_color_black:
            dev->pStrip->set_pixel(dev->pStrip,0,0,0,0);//black
            dev->pStrip->refresh(dev->pStrip, 100);
            dev->is_setcolor_ft = false;
            break;
        default:
            dev->pStrip->set_pixel(dev->pStrip,0,0,0,0);//black
            dev->pStrip->refresh(dev->pStrip, 100);
            dev->is_setcolor_ft = false;
            break;
    }                                    
}

void reset_rgb_led(rgbled_t* dev) {
    // printf("reset_rgb_led ...dev->is_setcolor_ft: %s\n\n", dev->is_setcolor_ft ? "true" : "false");
    // printf("reset_rgb_led ...dev->is_blink_ft: %s\n\n", dev->is_blink_ft ? "true" : "false");
    // printf("reset_rgb_led ...dev->is_pulse_blink_ft: %s\n\n", dev->is_pulse_blink_ft ? "true" : "false");

    //gpio_set_level(dev->en_pin, false);

    if (!dev->is_setcolor_ft) {
        dev->pStrip->clear(dev->pStrip, 100);   //Set all LED off to clear all pixels
        //printf("reset ...dev->is_blink_ft\n\n");
        dev->is_setcolor_ft = true;
        // dev->current_led_color = NULL;
        // dev->current_led_mode = NULL;
        gpio_set_level(dev->en_pin, false);
    }
    if (!dev->is_blink_ft) {
        vTaskDelete(prvblink_color_rgb_led_taskHandle);
        vTaskDelay(100 / portTICK_PERIOD_MS);
        //printf("reset ...dev->is_blink_ft\n\n");
        dev->is_blink_ft = true;
        // dev->current_led_color = NULL;
        // dev->current_led_mode = NULL;
        gpio_set_level(dev->en_pin, false);
    }
    if (!dev->is_pulse_blink_ft) {
        vTaskDelete(prvpulse_blink_color_rgb_led_taskHandle);
        vTaskDelay(100 / portTICK_PERIOD_MS);
        //printf("reset...dev->is_pulse_blink_ft\n\n");
        dev->is_pulse_blink_ft = true;
        // dev->current_led_color = NULL;
        // dev->current_led_mode = NULL;
        gpio_set_level(dev->en_pin, false);
    }
    

}

void reset_blink_color_rgb_led (rgbled_t* dev) {
    reset_rgb_led(dev);

}

void reset_pulse_blink_color_rgb_led (rgbled_t* dev) {
    reset_rgb_led(dev);
}







static void prvblink_color_rgb_led_task( void * pvParameters ) {

    TaskParameters *parameters= (TaskParameters *)pvParameters;
    // if ((parameters = malloc (sizeof(TaskParameters))) == NULL)
    //     return;

    static uint8_t blink_led_state = 0;
    led_color_t color = parameters->led_color;

    while (1) {
        if (blink_led_state) {
            switch (color) {
                case led_color_red:
                    parameters->dev->pStrip->set_pixel(parameters->dev->pStrip,0,255,0,0);//RED
                    parameters->dev->pStrip->refresh(parameters->dev->pStrip, 100);
                    break;
                case led_color_green:
                    parameters->dev->pStrip->set_pixel(parameters->dev->pStrip,0,0,255,0);//GEEN
                    parameters->dev->pStrip->refresh(parameters->dev->pStrip, 100);
                    break;
                case led_color_blue:
                    parameters->dev->pStrip->set_pixel(parameters->dev->pStrip,0,0,0,255);//BLUE
                    parameters->dev->pStrip->refresh(parameters->dev->pStrip, 100);
                    break;
                case led_color_yellow:
                    parameters->dev->pStrip->set_pixel(parameters->dev->pStrip,0,255,255,0);//Yellow
                    parameters->dev->pStrip->refresh(parameters->dev->pStrip, 100);
                    break;
                case led_color_orange:
                    parameters->dev->pStrip->set_pixel(parameters->dev->pStrip,0,255,127,0);//Orange
                    parameters->dev->pStrip->refresh(parameters->dev->pStrip, 100);
                    break;
                case led_color_purple:
                    parameters->dev->pStrip->set_pixel(parameters->dev->pStrip,0,255,0,255);//purple
                    parameters->dev->pStrip->refresh(parameters->dev->pStrip, 100);
                    break;
                case led_color_white:
                    parameters->dev->pStrip->set_pixel(parameters->dev->pStrip,0,255,255,255);//white
                    parameters->dev->pStrip->refresh(parameters->dev->pStrip, 100);
                    break;
                case led_color_black:
                    parameters->dev->pStrip->set_pixel(parameters->dev->pStrip,0,0,0,0);//black
                    parameters->dev->pStrip->refresh(parameters->dev->pStrip, 100);
                    break;
                default:
                    parameters->dev->pStrip->set_pixel(parameters->dev->pStrip,0,0,0,0);//black
                    parameters->dev->pStrip->refresh(parameters->dev->pStrip, 100);
                    break;
            }     
        } else {
           parameters->dev->pStrip->clear(parameters->dev->pStrip, 50); //Set all LED off to clear all pixels
        }
        blink_led_state = !blink_led_state; //Toggle the LED state
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}



static void prvpulse_blink_color_rgb_led_task( void * pvParameters ) {
    TaskParameters *parameters= (TaskParameters *)pvParameters;
    // if ((parameters = malloc (sizeof(TaskParameters))) == NULL)
    //     return;

    static uint8_t blink_led_state = 0;
    led_color_t color = parameters->led_color;
    
    static uint8_t pulse_led_state = 0;
    uint16_t multi=10,r=0,g=0,b=0;

    switch (color) {
        case led_color_red:
            while (1) {
                if (!pulse_led_state) {
                    for (uint16_t i = 0; i < 245; i++) {
                        r=i+multi;
                        parameters->dev->pStrip->set_pixel(parameters->dev->pStrip,0,r,0,0);//red
                        parameters->dev->pStrip->refresh(parameters->dev->pStrip, 100);
                        vTaskDelay(15 / portTICK_PERIOD_MS);  
                    }
                    pulse_led_state = !pulse_led_state; //Toggle the LED state
                } else {
                    for (uint16_t i = 255; i > 10; i--) {
                        r=i-multi;
                        parameters->dev->pStrip->set_pixel(parameters->dev->pStrip,0,r,0,0);//white
                        parameters->dev->pStrip->refresh(parameters->dev->pStrip, 100);
                        vTaskDelay(15 / portTICK_PERIOD_MS);       
                    }
                    pulse_led_state = !pulse_led_state; //Toggle the LED state
                }
            }
            break;
        case led_color_green:
            while (1) {
                if (!pulse_led_state) {
                    for (uint16_t i = 0; i < 245; i++) {
                        g=i+multi;
                        parameters->dev->pStrip->set_pixel(parameters->dev->pStrip,0,0,g,0);//green
                        parameters->dev->pStrip->refresh(parameters->dev->pStrip, 100);
                        vTaskDelay(15 / portTICK_PERIOD_MS);  
                    }
                    pulse_led_state = !pulse_led_state; //Toggle the LED state
                } else {
                    for (uint16_t i = 255; i > 10; i--) {
                        g=i-multi;
                        parameters->dev->pStrip->set_pixel(parameters->dev->pStrip,0,0,g,0);//green
                        parameters->dev->pStrip->refresh(parameters->dev->pStrip, 100);
                        vTaskDelay(15 / portTICK_PERIOD_MS);       
                    }
                    pulse_led_state = !pulse_led_state; //Toggle the LED state
                }
            }
            break;
        case led_color_blue:
            while (1) {
                if (!pulse_led_state) {
                    for (uint16_t i = 0; i < 245; i++) {
                        b=i+multi;
                        parameters->dev->pStrip->set_pixel(parameters->dev->pStrip,0,0,0,b);//blue
                        parameters->dev->pStrip->refresh(parameters->dev->pStrip, 100);
                        vTaskDelay(15 / portTICK_PERIOD_MS);  
                    }
                    pulse_led_state = !pulse_led_state; //Toggle the LED state
                } else {
                    for (uint16_t i = 255; i > 10; i--) {
                        b=i-multi;
                        parameters->dev->pStrip->set_pixel(parameters->dev->pStrip,0,0,0,b);//blue
                        parameters->dev->pStrip->refresh(parameters->dev->pStrip, 100);
                        vTaskDelay(15 / portTICK_PERIOD_MS);       
                    }
                    pulse_led_state = !pulse_led_state; //Toggle the LED state
                }
            }
            break;
        case led_color_yellow:
            while (1) {
                if (!pulse_led_state) {
                    for (uint16_t i = 0; i < 245; i++) {
                        r=i+multi; g=i+multi;
                        parameters->dev->pStrip->set_pixel(parameters->dev->pStrip,0,r,g,0);//yellow
                        parameters->dev->pStrip->refresh(parameters->dev->pStrip, 100);
                        vTaskDelay(15 / portTICK_PERIOD_MS);  
                    }
                    pulse_led_state = !pulse_led_state; //Toggle the LED state
                } else {
                    for (uint16_t i = 255; i > 10; i--) {
                        r=i-multi; g=i+multi;
                        parameters->dev->pStrip->set_pixel(parameters->dev->pStrip,0,r,g,0);//yellow
                        parameters->dev->pStrip->refresh(parameters->dev->pStrip, 100);
                        vTaskDelay(15 / portTICK_PERIOD_MS);       
                    }
                    pulse_led_state = !pulse_led_state; //Toggle the LED state
                }
            }
            break;
        case led_color_orange:
            while (1) {
                if (!pulse_led_state) {
                    for (uint16_t i = 0; i < 245; i++) {
                        r=i+multi;
                        if (i <= 127)
                            g=i+multi;
                        parameters->dev->pStrip->set_pixel(parameters->dev->pStrip,0,r,g,0);//orange
                        parameters->dev->pStrip->refresh(parameters->dev->pStrip, 100);
                        vTaskDelay(15 / portTICK_PERIOD_MS);  
                    }
                    pulse_led_state = !pulse_led_state; //Toggle the LED state
                } else {
                    g=127;
                    for (uint16_t i = 255; i > 10; i--) {
                        r=i-multi; 
                        if (i >= 127)
                            g=i-multi;
                        parameters->dev->pStrip->set_pixel(parameters->dev->pStrip,0,r,g,0);//orange
                        parameters->dev->pStrip->refresh(parameters->dev->pStrip, 100);
                        vTaskDelay(15 / portTICK_PERIOD_MS);       
                    }
                    pulse_led_state = !pulse_led_state; //Toggle the LED state
                }
            }
            break;
        case led_color_purple:
            while (1) {
                if (!pulse_led_state) {
                    for (uint16_t i = 0; i < 245; i++) {
                        r=i+multi; b=i+multi;
                        parameters->dev->pStrip->set_pixel(parameters->dev->pStrip,0,r,0,b);//purple
                        parameters->dev->pStrip->refresh(parameters->dev->pStrip, 100);
                        vTaskDelay(15 / portTICK_PERIOD_MS);  
                    }
                    pulse_led_state = !pulse_led_state; //Toggle the LED state
                } else {
                    for (uint16_t i = 255; i > 10; i--) {
                        r=i-multi; b=i+multi;
                        parameters->dev->pStrip->set_pixel(parameters->dev->pStrip,0,r,0,b);//purple
                        parameters->dev->pStrip->refresh(parameters->dev->pStrip, 100);
                        vTaskDelay(15 / portTICK_PERIOD_MS);       
                    }
                    pulse_led_state = !pulse_led_state; //Toggle the LED state
                }
            }
            break;
        case led_color_white:
            while (1) {
                if (!pulse_led_state) {
                    for (uint16_t i = 0; i < 245; i++) {
                        r=i+multi; g=i+multi; b=i+multi;
                        parameters->dev->pStrip->set_pixel(parameters->dev->pStrip,0,r,g,b);//white
                        parameters->dev->pStrip->refresh(parameters->dev->pStrip, 100);
                        vTaskDelay(15 / portTICK_PERIOD_MS);  
                    }
                    pulse_led_state = !pulse_led_state; //Toggle the LED state
                } else {
                    for (uint16_t i = 255; i > 10; i--) {
                        r=i-multi; g=i-multi; b=i-multi;
                        parameters->dev->pStrip->set_pixel(parameters->dev->pStrip,0,r,g,b);//white
                        parameters->dev->pStrip->refresh(parameters->dev->pStrip, 100);
                        vTaskDelay(15 / portTICK_PERIOD_MS);       
                    }
                    pulse_led_state = !pulse_led_state; //Toggle the LED state
                }
            }
            break;
        case led_color_black:
            while (1) {
                parameters->dev->pStrip->set_pixel(parameters->dev->pStrip,0,0,0,0);//black
                parameters->dev->pStrip->refresh(parameters->dev->pStrip, 100);
            }
            break;
        default:
            while (1) {
                parameters->dev->pStrip->set_pixel(parameters->dev->pStrip,0,0,0,0);//black
                parameters->dev->pStrip->refresh(parameters->dev->pStrip, 100);
            }
            break;
    }
}



static void set_blink_color_rgb_led(rgbled_t* dev, led_color_t color) {
    TaskParameters* parameters;
    if ((parameters = malloc (sizeof(TaskParameters))) == NULL)
        return;
    
    parameters->dev = dev;
    parameters->led_color = color;

    if (!dev->is_setcolor_ft || !dev->is_blink_ft || !dev->is_pulse_blink_ft) {
        reset_rgb_led(dev);
        gpio_set_level(dev->en_pin, true);
        xTaskCreate(&prvblink_color_rgb_led_task,                 //Function that implements the task.
            "prvblink_color_rgb_led_task",            //Text name for the task - only used for debugging.
            1024 * 4,                                   //Size of stack (in words, not bytes) to allocate for the task.
            (void *)parameters,                                //Task parameter - not used in this case.
            5,                                          //Task priority, must be between 0 and configMAX_PRIORITIES - 1.
            &prvblink_color_rgb_led_taskHandle);       //Used to pass out a handle to the created task - not used in this case.
        dev->is_blink_ft = false;
    } else {
        gpio_set_level(dev->en_pin, true);
        xTaskCreate(&prvblink_color_rgb_led_task,                 //Function that implements the task.
                "prvblink_color_rgb_led_task",            //Text name for the task - only used for debugging.
                1024 * 4,                                   //Size of stack (in words, not bytes) to allocate for the task.
                (void *)parameters,                               //Task parameter - not used in this case.
                5,                                          //Task priority, must be between 0 and configMAX_PRIORITIES - 1.
                &prvblink_color_rgb_led_taskHandle);       //Used to pass out a handle to the created task - not used in this case.
        dev->is_blink_ft = false;

    }
}



static void set_pulse_blink_color_rgb_led(rgbled_t* dev, led_color_t color) {
    TaskParameters* parameters;
    if ((parameters = malloc (sizeof(TaskParameters))) == NULL)
        return;
    
    parameters->dev = dev;
    parameters->led_color = color;

    if (!dev->is_setcolor_ft || !dev->is_blink_ft || !dev->is_pulse_blink_ft) {
        reset_rgb_led(dev);
        gpio_set_level(dev->en_pin, true);
        xTaskCreate(&prvpulse_blink_color_rgb_led_task,                 //Function that implements the task.
            "prvpulse_blink_color_rgb_led_task",            //Text name for the task - only used for debugging.
            1024 * 4,                                   //Size of stack (in words, not bytes) to allocate for the task.
            (void *)parameters,                                //Task parameter - not used in this case.
            5,                                          //Task priority, must be between 0 and configMAX_PRIORITIES - 1.
            &prvpulse_blink_color_rgb_led_taskHandle);       //Used to pass out a handle to the created task - not used in this case.
        dev->is_pulse_blink_ft = false;
    } else {
        gpio_set_level(dev->en_pin, true);
        xTaskCreate(&prvpulse_blink_color_rgb_led_task,                 //Function that implements the task.
                "prvpulse_blink_color_rgb_led_task",            //Text name for the task - only used for debugging.
                1024 * 4,                                   //Size of stack (in words, not bytes) to allocate for the task.
                (void *)parameters,                               //Task parameter - not used in this case.
                5,                                          //Task priority, must be between 0 and configMAX_PRIORITIES - 1.
                &prvpulse_blink_color_rgb_led_taskHandle);       //Used to pass out a handle to the created task - not used in this case.
        dev->is_pulse_blink_ft = false;

    }
}









// void del_task (void) {
//     // ESP_LOGE(TAG,"deleting task....");
//     // vTaskDelete(prvblink_color_rgb_led_taskHandle);
//     // dev->is_blink_ft = true;
// }

// void get_state (void) {

//         // vTaskGetInfo( 
//         //           prvblink_color_rgb_led_taskHandle,          //The handle of the task being queried.
//         //           &prvblink_color_rgb_led_taskDetails,    //The TaskStatus_t structure to complete with information on xTask.
//         //           pdTRUE,           //Include the stack high water mark value in the TaskStatus_t structure.
//         //           eInvalid );       //Include the task state in the TaskStatus_t structure.
//         // ESP_LOGE(TAG,"vTaskGetInfo: %d" , prvblink_color_rgb_led_taskDetails.eCurrentState);

// }
