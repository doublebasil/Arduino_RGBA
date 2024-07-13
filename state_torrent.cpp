#include "state_torrent.hpp"
#include "intcos.hpp"

#define ROTATION_DEGREES_PER_SECOND ( 10 )
#define MS_PER_SECOND               ( 1000UL )
#define DEGREES_PER_ROTATION        ( 360UL )

void StateTorrent::button_press_action(void) const
{
    // Change to thermal
    StateAbstract::current_state = sm_thermal;
}
void StateTorrent::get_led_states(CRGB *front_fans_leds) const
{
    const uint16_t add_deg = ((StateAbstract::ms_since_boot * ROTATION_DEGREES_PER_SECOND) / MS_PER_SECOND) % DEGREES_PER_ROTATION;

    for (uint8_t led_index = 0U; led_index < FRONT_FANS_NUM_LEDS; ++led_index)
    {
        const uint16_t deg = (front_fan_degrees[led_index] + add_deg) % 360;
        front_fans_leds[led_index] = blend(light_colour, dark_colour, intcos(deg));
    }
}
void StateTorrent::check_if_state_should_change(void) const
{
    pc_state_t current_pc_state = get_pc_state();
    if (current_pc_state == pc_off)
        StateAbstract::current_state = sm_system_off;
}
