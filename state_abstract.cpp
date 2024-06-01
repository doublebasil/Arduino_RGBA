#include "state_abstract.hpp"

uint64_t StateAbstract::ms_since_boot = 0;
system_state_t StateAbstract::current_state = sm_system_off;
system_state_t StateAbstract::previous_state = sm_system_off;

uint16_t StateAbstract::cpu_cooler_inner_degrees[CPU_COOLER_INNER_NUM_LEDS];
uint16_t StateAbstract::cpu_cooler_outer_degrees[CPU_COOLER_OUTER_NUM_LEDS];
uint16_t StateAbstract::led_strip_degrees[LED_STRIP_NUM_LEDS];
uint16_t StateAbstract::front_fan_degrees[FRONT_FANS_NUM_LEDS];

StateAbstract::StateAbstract()
{
    // Calculate the approximate angle each of the LEDs will be at (used for gradients)
    for (uint8_t led_number = 0U; led_number < CPU_COOLER_INNER_NUM_LEDS; ++led_number)
    {
        StateAbstract::cpu_cooler_inner_degrees[led_number] = (360UL * led_number) / CPU_COOLER_INNER_NUM_LEDS;
    }
    for (uint8_t led_number = 0U; led_number < CPU_COOLER_OUTER_NUM_LEDS; ++led_number)
    {
        StateAbstract::cpu_cooler_outer_degrees[led_number] = (360UL * led_number) / CPU_COOLER_OUTER_NUM_LEDS;
    }
    for (uint8_t led_number = 0U; led_number < LED_STRIP_NUM_LEDS; ++led_number)
    {
        StateAbstract::led_strip_degrees[led_number] = (180UL * led_number) / LED_STRIP_NUM_LEDS;
    }
    for (uint8_t led_number = 0U; led_number < FRONT_FANS_NUM_LEDS; ++led_number)
    {
        StateAbstract::front_fan_degrees[led_number] = (360UL * led_number) / FRONT_FANS_NUM_LEDS;
    }
}
