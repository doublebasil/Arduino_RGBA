#include "Arduino.h"
#include "state_thermal.hpp"
#include "intcos.hpp"
#include "temperature_sensor.hpp"

#define ROTATION_DEGREES_PER_SECOND ( 2 )
#define MS_PER_SECOND               ( 1000UL )
#define DEGREES_PER_ROTATION        ( 360UL )

void StateThermal::button_press_action(void) const
{
    // Change to static red
    StateAbstract::current_state = sm_static_red;
}
void StateThermal::get_led_states(CRGB *cpu_leds, CRGB *led_strip_leds, CRGB *front_fans_leds) const
{
    float current_temp = get_temperature();
    uint8_t temperature_blend_val;
    if (current_temp < this->low_temp)
        temperature_blend_val = 0x00;
    else if (current_temp > this->high_temp)
        temperature_blend_val = 0xFF;
    else
    {
        float working_val = (current_temp - this->low_temp) * (float) 0xFF;
        working_val /= (this->high_temp - this->low_temp);
        temperature_blend_val = (uint8_t) round(working_val);
    }
    // Serial.print("Current temp = ");
    // Serial.print(current_temp);
    // Serial.print(", temperature_blend_val = ");
    // Serial.println(temperature_blend_val);

    const CRGB light_colour = blend(cool_colour_light, hot_colour_light, temperature_blend_val);
    const CRGB dark_colour = blend(cool_colour_dark, hot_colour_dark, temperature_blend_val);

    const uint16_t add_deg = ((StateAbstract::ms_since_boot * ROTATION_DEGREES_PER_SECOND) / MS_PER_SECOND) % DEGREES_PER_ROTATION;

    for (uint8_t led_index = 0U; led_index < CPU_COOLER_INNER_NUM_LEDS; ++led_index)
    {
        const uint16_t deg = (cpu_cooler_inner_degrees[led_index] + add_deg) % 360;
        cpu_leds[led_index] = blend(light_colour, dark_colour, intcos(deg));
    }
    for (uint8_t led_index = 0U; led_index < CPU_COOLER_OUTER_NUM_LEDS; ++led_index)
    {
        const uint16_t deg = (cpu_cooler_outer_degrees[led_index] + add_deg) % 360;
        cpu_leds[CPU_COOLER_INNER_NUM_LEDS + led_index] = blend(light_colour, dark_colour, intcos(deg));
    }
    for (uint8_t led_index = 0U; led_index < LED_STRIP_NUM_LEDS; ++led_index)
    {
        led_strip_leds[led_index] = blend(light_colour, dark_colour, intcos(led_strip_degrees[led_index]));
    }
    for (uint8_t led_index = 0U; led_index < FRONT_FANS_NUM_LEDS; ++led_index)
    {
        const uint16_t deg = (front_fan_degrees[led_index] + add_deg) % 360;
        front_fans_leds[led_index] = blend(light_colour, dark_colour, intcos(deg));
    }
}
void StateThermal::check_if_state_should_change(void) const
{
    pc_state_t current_pc_state = get_pc_state();
    if (current_pc_state == pc_off)
        StateAbstract::current_state = sm_system_off;
}
