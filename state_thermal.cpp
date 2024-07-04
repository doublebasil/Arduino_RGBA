#include "Arduino.h"
#include "state_thermal.hpp"
#include "intcos.hpp"
#include "fan_speed.hpp"

#define ROTATION_DEGREES_PER_SECOND ( 2 )
#define MS_PER_SECOND               ( 1000UL )
#define DEGREES_PER_ROTATION        ( 360UL )

#define LOADING_BAR_STYLE_STRIP     ( 1 )
#define BLENDED_STRIP_AVERAGES      ( 8 )

void StateThermal::button_press_action(void) const
{
    // Change to static red
    StateAbstract::current_state = sm_static_red;
}
void StateThermal::get_led_states(CRGB *cpu_leds, CRGB *led_strip_leds, CRGB *front_fans_leds) const
{
    const uint16_t map_from_low = 300;
    const uint16_t map_from_high = 550;
    const uint16_t map_to_low = 0;
    const uint16_t map_to_high = 511;
    
    uint16_t temperature_scale_val;
    const uint16_t current_fan_speed = get_fan_speed();
    if (current_fan_speed <= map_from_low)
    {
        temperature_scale_val = map_to_low;
    }
    else if (current_fan_speed >= map_from_high)
    {
        temperature_scale_val = map_to_high;
    }
    else
    {
        temperature_scale_val = map(current_fan_speed, map_from_low, map_from_high, map_to_low, map_to_high);
    }

    uint8_t temperature_blend_val;
    if (temperature_scale_val > 255)
    {
        temperature_blend_val = ((temperature_scale_val - 0xFF) > 0xFF) ? 0xFF : temperature_scale_val - 0xFF;
    }
    else
    {
        temperature_blend_val = temperature_scale_val;
    }

    CRGB light_colour;
    CRGB dark_colour;

    if (temperature_scale_val < 256)
    {
        light_colour = blend(cool_colour_light, med_colour_light, temperature_blend_val);
        dark_colour = blend(cool_colour_dark, med_colour_dark, temperature_blend_val);
    }
    else
    {
        light_colour = blend(med_colour_light, hot_colour_light, temperature_blend_val);
        dark_colour = blend(med_colour_dark, hot_colour_dark, temperature_blend_val);
    }

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
    for (uint8_t led_index = 0U; led_index < FRONT_FANS_NUM_LEDS; ++led_index)
    {
        const uint16_t deg = (front_fan_degrees[led_index] + add_deg) % 360;
        front_fans_leds[led_index] = blend(light_colour, dark_colour, intcos(deg));
    }

    
#   if defined(LOADING_BAR_STYLE_STRIP) && (LOADING_BAR_STYLE_STRIP == 1)
    int8_t strip_leds_on = ((uint32_t) temperature_scale_val * (uint32_t) LED_STRIP_NUM_LEDS) / 511ULL;
    const uint8_t blended_strip_led_amount = (((uint32_t) temperature_scale_val * (uint32_t) LED_STRIP_NUM_LEDS) % 511ULL) / 2;
    int8_t current_led = LED_STRIP_NUM_LEDS - 1;

    static uint8_t blended_strip_led_values[BLENDED_STRIP_AVERAGES];
    static bool is_first_loop = true;
    static uint8_t blended_strip_led_values_index = 0;
    static uint8_t previous_strip_leds_on = strip_leds_on;

    if ((is_first_loop == true) || (strip_leds_on != previous_strip_leds_on))
    {
        for (uint8_t index = 0; index < BLENDED_STRIP_AVERAGES; ++index)
        {
            blended_strip_led_values[index] = blended_strip_led_amount;
        }
        is_first_loop = false;
    }
    else
    {
        blended_strip_led_values[blended_strip_led_values_index] = blended_strip_led_amount;
        ++blended_strip_led_values_index;
        if (blended_strip_led_values_index == BLENDED_STRIP_AVERAGES)
        {
            blended_strip_led_values_index = 0;
        }
    }

    uint32_t averaged_blended_value = 0;
    for (uint8_t index = 0; index < BLENDED_STRIP_AVERAGES; ++index)
    {
        averaged_blended_value += blended_strip_led_values[index];
    }
    averaged_blended_value /= BLENDED_STRIP_AVERAGES;
    if (averaged_blended_value > 255)
    {
        averaged_blended_value = 255;
    }

    while (current_led >= 0)
    {
        if (strip_leds_on > 0)
        {
            // This LED is completely on
            led_strip_leds[current_led] = blend(light_colour, dark_colour, intcos(led_strip_degrees[current_led]));
        }
        else if (strip_leds_on == 0)
        {
            // This LED is somewhere between on and off
            CRGB on_colour = blend(light_colour, dark_colour, intcos(led_strip_degrees[current_led]));
            led_strip_leds[current_led] = blend(CRGB(0, 0, 0), on_colour, averaged_blended_value);
        }
        else
        {
            // This LED is completely off
            led_strip_leds[current_led] = CRGB(0, 0, 0);
        }

        --strip_leds_on;
        --current_led;
    }
    previous_strip_leds_on = strip_leds_on;

#   else
    for (uint8_t led_index = 0U; led_index < LED_STRIP_NUM_LEDS; ++led_index)
    {
        led_strip_leds[led_index] = blend(light_colour, dark_colour, intcos(led_strip_degrees[led_index]));
    }
#   endif
}
void StateThermal::check_if_state_should_change(void) const
{
    pc_state_t current_pc_state = get_pc_state();
    if (current_pc_state == pc_off)
        StateAbstract::current_state = sm_system_off;
}
