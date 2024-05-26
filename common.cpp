#include <Arduino.h>
#include "common.hpp"

pc_state_t get_pc_state(void)
{
    int analog_reading = analogRead(POWER_BUTTON_ADC_PIN);
    if (analog_reading < POWER_BUTTON_ADC_THRESHOLD)
        return pc_on;
    else
        return pc_off;
}

void copy_led_data(CRGB *src, CRGB *dest, uint8_t len)
{
    for (uint8_t index = 0U; index < len; ++index)
    {
        dest[index].red = src[index].red;
        dest[index].green = src[index].green;
        dest[index].blue = src[index].blue;
    }
}
