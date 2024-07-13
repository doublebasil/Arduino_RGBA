#ifndef STATE_ABSTRACT_HPP
#define STATE_ABSTRACT_HPP

#include "FastLED.h"
#include <stdint.h>
#include "common.hpp"

class StateAbstract
{
public:
    StateAbstract();
    virtual void button_press_action(void) const = 0;
    virtual void get_led_states(CRGB front_fans_leds[]) const = 0;
    virtual void check_if_state_should_change(void) const = 0;

    static uint64_t ms_since_boot;
    static system_state_t current_state;  // SM
    static system_state_t previous_state; // SM

    static uint16_t front_fan_degrees[FRONT_FANS_NUM_LEDS];
};

#endif // ndef STATE_ABSTRACT_HPP
