#ifndef STATE_TORRENT_HPP
#define STATE_TORRENT_HPP

#include "FastLED.h"
#include <stdint.h>
#include "state_abstract.hpp"
#include "common.hpp"

class StateTorrent : public StateAbstract
{
public:
    void button_press_action(void) const override;
    void get_led_states(CRGB front_fans_leds[]) const override;
    void check_if_state_should_change(void) const override;
private:
    // const CRGB light_colour = CRGB(0, 200, 40);
    // const CRGB dark_colour = CRGB(0, 0, 200);
    const CRGB light_colour = CRGB(200, 0, 200);
    const CRGB dark_colour = CRGB(200, 0, 0);
};

#endif // ndef STATE_TORRENT_HPP