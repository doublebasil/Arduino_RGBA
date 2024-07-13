#include "state_static.hpp"

void StateStatic::button_press_action(void) const
{
    // Change to Torrent
    StateAbstract::current_state = sm_torrent;
}
void StateStatic::get_led_states(CRGB *front_fans_leds) const
{
    for (uint8_t led_index = 0; led_index < FRONT_FANS_NUM_LEDS; ++led_index)
    {
        front_fans_leds[led_index] = CRGB(50, 0, 0);
    }
}
void StateStatic::check_if_state_should_change(void) const
{
    pc_state_t current_pc_state = get_pc_state();
    if (current_pc_state == pc_off)
        StateAbstract::current_state = sm_system_off;
}
