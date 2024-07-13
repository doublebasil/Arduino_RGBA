#include "FastLED.h" // FastLED NeoPixel by David Madison

#include "common.hpp"

#include "state_abstract.hpp"
#include "state_off.hpp"
#include "state_static.hpp"
#include "state_torrent.hpp"

/*
LED Layout for my ThermalTake build

                      1  2  3  4  5  6  7  8
140mm Gigabyte Fans   ## ## ## ## ## ## ## ##

*/

CRGB front_fans_leds[FRONT_FANS_NUM_LEDS];

typedef enum
{
    button_pressed,
    button_not_pressed,
} button_state_t;

void system_update(void);

void setup()
{
    // If the Arduino reset due to the power drawn, this delay may help prevent damage
    delay(250);

    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(RESET_BUTTON_INPUT_PIN, INPUT_PULLUP);

    FastLED.addLeds<NEOPIXEL, FRONT_FANS_DATA_PIN>(front_fans_leds, FRONT_FANS_NUM_LEDS);

    // Serial.begin(9600);
}

void loop()
{
    static unsigned long loop_start_time = millis();

    unsigned long new_millis = millis();
    StateAbstract::ms_since_boot += new_millis - loop_start_time;
    loop_start_time = new_millis;

    system_update();

    unsigned long loop_duration = millis() - loop_start_time;
    // Serial.println(loop_duration);

    if (loop_duration < UPDATE_PERIOD_MS)
        delay(UPDATE_PERIOD_MS - loop_duration);

    // Serial.println(millis() - loop_start_time);
}

void system_update(void)
{
    static uint8_t transition_stage;
    static CRGB front_fans_transition_start[FRONT_FANS_NUM_LEDS];

    static StateOff state_off_obj;
    static StateStatic state_static_obj;
    static StateTorrent state_torrent_obj;

    static button_state_t current_button_state;
    static button_state_t previous_button_state = button_not_pressed;

    current_button_state = (digitalRead(RESET_BUTTON_INPUT_PIN) == LOW) ? button_pressed : button_not_pressed;
    if ((current_button_state == button_pressed) && (previous_button_state == button_not_pressed))
    {
        switch(StateAbstract::current_state)
        {
            case sm_static:
            {
                state_static_obj.button_press_action();
            }
            break;
            case sm_torrent:
            {
                state_torrent_obj.button_press_action();
            }
            break;
            case sm_system_off:
            default:
            {
                state_off_obj.button_press_action();
            }
            break;        
        }
    }
    previous_button_state = current_button_state;

    if (StateAbstract::current_state != StateAbstract::previous_state)
    {
        // Begin a transition
        transition_stage = TRANSITION_CYCLES;
        StateAbstract::previous_state = StateAbstract::current_state;

        copy_led_data(front_fans_leds, front_fans_transition_start, FRONT_FANS_NUM_LEDS);
    }

    CRGB front_fans_new_states[FRONT_FANS_NUM_LEDS];

    switch(StateAbstract::current_state)
    {
        case sm_static:
        {
            state_static_obj.get_led_states(front_fans_new_states);
            state_static_obj.check_if_state_should_change();
        }
        break;
        case sm_torrent:
        {
            state_torrent_obj.get_led_states(front_fans_new_states);
            state_torrent_obj.check_if_state_should_change();
        }
        break;
        case sm_system_off:
        default:
        {
            state_off_obj.get_led_states(front_fans_new_states);
            state_off_obj.check_if_state_should_change();
        }
        break;        
    }

    if (transition_stage == 0)
    {
        copy_led_data(front_fans_new_states, front_fans_leds, FRONT_FANS_NUM_LEDS);
    }
    else
    {
        const uint8_t transition_proportion = ((uint16_t) transition_stage * 0xFFUL) / (uint16_t) TRANSITION_CYCLES;

        for (uint8_t led_index = 0U; led_index < FRONT_FANS_NUM_LEDS; ++led_index)
        {
            front_fans_leds[led_index] = blend(front_fans_new_states[led_index], front_fans_transition_start[led_index], transition_proportion);
        }
    }

    if (transition_stage > 0)
        --transition_stage;

    FastLED.show();  
}
