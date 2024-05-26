#include "state_off.hpp"

void StateOff::button_press_action(void) const
{
    // Do nothing
}

void StateOff::get_led_states(CRGB cpu_leds[], CRGB led_strip_leds[], CRGB front_fans_leds[]) const
{
    for (uint8_t led_index = 0; led_index < CPU_COOLER_NUM_LEDS; ++led_index)
    {
        cpu_leds[led_index] = CRGB(0, 0, 0);
    }
    for (uint8_t led_index = 0; led_index < LED_STRIP_NUM_LEDS; ++led_index)
    {
        led_strip_leds[led_index] = CRGB(0, 0, 0);
    }
    for (uint8_t led_index = 0; led_index < FRONT_FANS_NUM_LEDS; ++led_index)
    {
        front_fans_leds[led_index] = CRGB(0, 0, 0);
    }
}

void StateOff::check_if_state_should_change(void) const
{
    pc_state_t current_pc_state = get_pc_state();
    if (current_pc_state == pc_on)
        StateOff::current_state = DEFAULT_MODE;
}
