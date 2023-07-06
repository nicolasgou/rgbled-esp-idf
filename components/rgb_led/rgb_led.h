// Copyright (c)IASX Technologies S/A All rights reserved.
// SPDX-License-Identifier: MIT


#include <stdio.h>
#include <stdlib.h>

#include <driver/gpio.h>


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "esp_log.h"

#include "led_strip.h"



typedef enum {
    led_color_none = 0,
    led_color_white,
    led_color_red,
    led_color_green,
    led_color_blue,
    led_color_purple,
    led_color_orange,
    led_color_yellow,
    led_color_black,
} led_color_t;

typedef enum {
    led_mode_none = 0,
    led_mode_static,
    led_mode_blink,
    led_mode_pulse,
} led_mode_t;

/**
 * @brief   RGBLED sensor device data structure type
 */
typedef struct {
    led_strip_t* pStrip;
    uint8_t data_pin; 
    uint8_t en_pin;
    led_color_t current_led_color;
    led_mode_t current_led_mode;
    bool is_setcolor_ft;
    bool is_blink_ft;
    bool is_pulse_blink_ft;
} rgbled_t;


typedef struct {
    rgbled_t* dev;
    led_color_t led_color;
} TaskParameters;

rgbled_t* initialize_rgb_led(uint8_t rgbLed_data_pin, uint8_t rgbLed_en_pin);
static void blink_rgb_initilize(rgbled_t* dev);


void set_dev_color_mode_rgbled (rgbled_t* dev, led_color_t color, led_mode_t mode);
static void set_color_rgb_led(rgbled_t* dev, led_color_t color);


void reset_rgb_led(rgbled_t* dev);
void reset_pulse_blink_color_rgb_led (rgbled_t* dev);
void reset_blink_color_rgb_led (rgbled_t* dev);


static void prvblink_color_rgb_led_task( void * pvParameters );

static void set_blink_color_rgb_led(rgbled_t* dev, led_color_t color);


static void prvpulse_blink_color_rgb_led_task( void * pvParameters );

static void set_pulse_blink_color_rgb_led(rgbled_t* dev, led_color_t color);


