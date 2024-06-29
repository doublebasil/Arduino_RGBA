#ifndef STATE_THERMAL_HPP
#define STATE_THERMAL_HPP

#include "FastLED.h"
#include <stdint.h>
#include "state_abstract.hpp"
#include "common.hpp"

class StateThermal : public StateAbstract
{
public:
    void button_press_action(void) const override;
    void get_led_states(CRGB cpu_leds[], CRGB led_strip_leds[], CRGB front_fans_leds[]) const override;
    void check_if_state_should_change(void) const override;
private:
    const CRGB hot_colour_dark = CRGB(255, 0, 0);
    const CRGB hot_colour_light = CRGB(150, 50, 0);
    const CRGB cool_colour_dark = CRGB(150, 150, 0);
    const CRGB cool_colour_light = CRGB(50, 50, 0);

    const float low_temp = 28.0;
    const float high_temp = 36.0;
};

#endif // ndef STATE_THERMAL_HPP