#include "state_abstract.hpp"

uint64_t StateAbstract::ms_since_boot = 0;
system_state_t StateAbstract::current_state = sm_system_off;
system_state_t StateAbstract::previous_state = sm_system_off;

uint16_t StateAbstract::front_fan_degrees[FRONT_FANS_NUM_LEDS];

StateAbstract::StateAbstract()
{
    // Calculate the approximate angle each of the LEDs will be at (used for gradients)
    for (uint8_t led_number = 0U; led_number < FRONT_FANS_NUM_LEDS; ++led_number)
    {
        StateAbstract::front_fan_degrees[led_number] = (360UL * led_number) / FRONT_FANS_NUM_LEDS;
    }
}
