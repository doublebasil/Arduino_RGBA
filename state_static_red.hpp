#ifndef STATE_STATIC_RED_HPP
#define STATE_STATIC_RED_HPP

#include "FastLED.h"
#include <stdint.h>
#include "state_abstract.hpp"
#include "common.hpp"

class StateStaticRed : public StateAbstract
{
public:
    void button_press_action(void) const override;
    void get_led_states(CRGB cpu_leds[], CRGB led_strip_leds[], CRGB front_fans_leds[]) const override;
    void check_if_state_should_change(void) const override;
};

#endif // ndef STATE_STATIC_RED_HPP
