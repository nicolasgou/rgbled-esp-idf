#include <stdio.h>
#include "rgb_led.h"

/* Private macro -------------------------------------------------------------*/

//LED Data IN OUTPUT GPIO PIN 
#define BUILTIN_BOARD_RGBLED_00_GPIO            15    // 15 -> WLSVS3M01 GPIO PIN | 48 -> devKit
#define BUILTIN_BOARD_RGBLED_00_TYPE            RMT   //RMT for RGB led Strip | GPIO (simple/mono color) for gpio led

//LED ENable OUTPUT GPIO PIN 
#define BUILTIN_BOARD_RGBLED_00_EN_GPIO            18    // This is the GPIO on which the output will be set
                                                   // 18 -> WLSVS2M01 LED EN | NOT -> devkit




/* Private variables ---------------------------------------------------------*/
rgbled_t* rgbled_00;
/* Private variables ---------------------------------------------------------*/

/* Extern variables ----------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/


/* app Main --------------------------------------------------------------*/
void app_main(void)
{
    rgbled_00 = initialize_rgb_led (BUILTIN_BOARD_RGBLED_00_GPIO, BUILTIN_BOARD_RGBLED_00_EN_GPIO);

    while (1)
    {
        set_dev_color_mode_rgbled(rgbled_00, led_color_red, led_mode_pulse );
        vTaskDelay(10000 / portTICK_PERIOD_MS);
        set_dev_color_mode_rgbled(rgbled_00, led_color_green, led_mode_blink );
        vTaskDelay(10000 / portTICK_PERIOD_MS);
        set_dev_color_mode_rgbled(rgbled_00, led_color_blue, led_mode_static );
        vTaskDelay(10000 / portTICK_PERIOD_MS);
    }


}
