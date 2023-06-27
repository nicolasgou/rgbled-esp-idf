idf_component_register(SRCS "rgb_led.c"
                    INCLUDE_DIRS "."
                    PRIV_REQUIRES "driver" "led_strip"
)
