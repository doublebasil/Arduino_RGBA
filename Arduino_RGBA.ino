#include "FastLED.h"
#include "intcos.hpp"

#define ANALOG_READ_PIN  ( A0 )

#define CPU_COOLER_DATA_PIN ( 12 )
#define LED_STRIP_DATA_PIN  ( 11 )
#define FRONT_FANS_DATA_PIN ( 10 )

#define ANALOG_THRESHOLD ( 300 )

#define UPDATE_PERIOD_MS ( 50 )
#define TRANSITION_CYCLES ( 10 )

#define CPU_COOLER_NUM_LEDS ( 24 )
#define CPU_COOLER_INNER_NUM_LEDS ( 8 )
#define CPU_COOLER_OUTER_NUM_LEDS ( 16 )
#define LED_STRIP_NUM_LEDS  ( 7 )
#define FRONT_FANS_NUM_LEDS ( 12 )

#define DEFAULT_MODE  ( sm_torrent )

#define DEBUG ( 0 )
// 0 = No Debug
// 1 = Count LEDs

/*
LED Layout for my Fractal Torrent build

                                         1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24
Case LED strip                        : ## ## ## ## ## ## ##
Cooler master CPU cooler (inner ring) : ## ## ## ## ## ## ## ##
Cooler master CPU cooler (outer ring) :                         ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ##
180mm Front Fans                      : ## ## ## ## ## ## ## ## ## ## ## ##

*/

typedef enum
{
    pc_on,
    pc_off,
} pc_state_t;

typedef enum
{
    sm_system_off,
    sm_static_red,
    sm_thermal,
    sm_torrent,
    sm_night,
} system_state_t;

// typedef struct
// {
//     uint8_t red;
//     uint8_t green;
//     uint8_t blue;
// } pixel_t;

CRGB cpu_cooler_leds[CPU_COOLER_NUM_LEDS];
CRGB led_strip_leds[LED_STRIP_NUM_LEDS];
CRGB front_fans_leds[FRONT_FANS_NUM_LEDS];

system_state_t sm_current_state;
system_state_t sm_previous_state;

pc_state_t get_pc_state(void);
void system_set_state(system_state_t new_state);
// void system_wake(void);
// void system_sleep(void);
void system_update(void);
// void temp(void);
// void leds_off(void);
void copy_led_data(CRGB src[], CRGB dest[], uint8_t len);

#if defined(DEBUG) && (DEBUG != 0)
void debug(void);
#endif

/* STATE ABSTRACT CLASS --------------------------------------------------------------------------- */
class StateAbstract
{
public:
    virtual void button_press_action(void) const = 0;
    virtual void get_led_states(CRGB cpu_leds[], CRGB led_strip_leds[], CRGB front_fans_leds[]) const = 0;
    virtual void check_if_state_should_change(void) const = 0;
};
/* OFF STATE -------------------------------------------------------------------------------------- */
class StateOff : public StateAbstract
{
public:
    void button_press_action(void) const override;
    void get_led_states(CRGB cpu_leds[], CRGB led_strip_leds[], CRGB front_fans_leds[]) const override;
    void check_if_state_should_change(void) const override;
};
void StateOff::button_press_action(void) const
{
    // Do nothing
}
void StateOff::get_led_states(CRGB cpu_leds[], CRGB led_strip_leds[], CRGB front_fans_leds[]) const
{
    for (uint8_t led_index = 0; led_index < CPU_COOLER_NUM_LEDS; ++led_index)
    {
        cpu_leds[led_index].red = 0;
        cpu_leds[led_index].green = 0;
        cpu_leds[led_index].blue = 0;
    }
    for (uint8_t led_index = 0; led_index < LED_STRIP_NUM_LEDS; ++led_index)
    {
        led_strip_leds[led_index].red = 0;
        led_strip_leds[led_index].green = 0;
        led_strip_leds[led_index].blue = 0;
    }
    for (uint8_t led_index = 0; led_index < LED_STRIP_NUM_LEDS; ++led_index)
    {
        front_fans_leds[led_index].red = 0;
        front_fans_leds[led_index].green = 0;
        front_fans_leds[led_index].blue = 0;
    }
}
void StateOff::check_if_state_should_change(void) const
{
    pc_state_t current_pc_state = get_pc_state();
    if (current_pc_state == pc_on)
        system_set_state(DEFAULT_MODE);
}
/* STATIC RED STATE ------------------------------------------------------------------------------- */
class StateStaticRed : public StateAbstract
{
public:
    void button_press_action(void) const override;
    void get_led_states(CRGB cpu_leds[], CRGB led_strip_leds[], CRGB front_fans_leds[]) const override;
    void check_if_state_should_change(void) const override;
};
void StateStaticRed::button_press_action(void) const
{
    // Do nothing (yet)
}
void StateStaticRed::get_led_states(CRGB *cpu_leds, CRGB *led_strip_leds, CRGB *front_fans_leds) const
{
    for (uint8_t led_index = 0; led_index < CPU_COOLER_NUM_LEDS; ++led_index)
    {
        cpu_leds[led_index].red = 200;
        cpu_leds[led_index].green = 0;
        cpu_leds[led_index].blue = 0;
    }
    for (uint8_t led_index = 0; led_index < LED_STRIP_NUM_LEDS; ++led_index)
    {
        led_strip_leds[led_index].red = 200;
        led_strip_leds[led_index].green = 0;
        led_strip_leds[led_index].blue = 0;
    }
    for (uint8_t led_index = 0; led_index < LED_STRIP_NUM_LEDS; ++led_index)
    {
        front_fans_leds[led_index].red = 200;
        front_fans_leds[led_index].green = 0;
        front_fans_leds[led_index].blue = 0;
    }
}
void StateStaticRed::check_if_state_should_change(void) const
{
    pc_state_t current_pc_state = get_pc_state();
    if (current_pc_state == pc_off)
        system_set_state(sm_system_off);
}


void setup()
{
    // Sanity check delay - If the Arduino reset due to the power drawn, this delay may help prevent damage
    delay(250);

    pinMode(LED_BUILTIN, OUTPUT);

    FastLED.addLeds<NEOPIXEL, CPU_COOLER_DATA_PIN>(cpu_cooler_leds, CPU_COOLER_NUM_LEDS);
    FastLED.addLeds<NEOPIXEL, LED_STRIP_DATA_PIN>(led_strip_leds, LED_STRIP_NUM_LEDS);
    FastLED.addLeds<NEOPIXEL, FRONT_FANS_DATA_PIN>(front_fans_leds, FRONT_FANS_NUM_LEDS);

    #if defined(DEBUG) && (DEBUG == 1)
    Serial.begin(9600);
    debug();
    #endif // debug == 1
}

void loop()
{
    unsigned long loop_start_time = millis();

    system_update();

    unsigned long loop_duration = millis() - loop_start_time;
    // Serial.println(loop_duration);
    if (loop_duration < UPDATE_PERIOD_MS)
        delay(UPDATE_PERIOD_MS - loop_duration);
}

pc_state_t get_pc_state(void)
{
    int analog_reading = analogRead(ANALOG_READ_PIN);
    if (analog_reading < ANALOG_THRESHOLD)
        return pc_on;
    else
        return pc_off;
}

void system_set_state(system_state_t new_state)
{
    sm_current_state = new_state;
}

void system_update(void)
{
    static uint8_t transition_stage;
    static CRGB cpu_cooler_transition_start[CPU_COOLER_NUM_LEDS];
    static CRGB led_strip_transition_start[LED_STRIP_NUM_LEDS];
    static CRGB front_fans_transition_start[FRONT_FANS_NUM_LEDS];

    static StateOff state_off_obj;
    static StateStaticRed state_static_red_obj;

    if (sm_current_state != sm_previous_state)
    {
        // Begin a transition
        transition_stage = TRANSITION_CYCLES;
        sm_previous_state = sm_current_state;

        copy_led_data(cpu_cooler_leds, cpu_cooler_transition_start, CPU_COOLER_NUM_LEDS);
        copy_led_data(led_strip_leds, led_strip_transition_start, CPU_COOLER_NUM_LEDS);
        copy_led_data(front_fans_leds, front_fans_transition_start, CPU_COOLER_NUM_LEDS);
    }

    CRGB cpu_cooler_new_states[CPU_COOLER_NUM_LEDS];
    CRGB led_strip_new_states[LED_STRIP_NUM_LEDS];
    CRGB front_fans_new_states[FRONT_FANS_NUM_LEDS];

    switch(sm_current_state)
    {
        case sm_static_red:
        {
            state_off_obj.get_led_states(cpu_cooler_new_states, led_strip_new_states, front_fans_new_states);
            state_off_obj.check_if_state_should_change();
        }
        break;
        case sm_thermal:
        {

        }
        break;
        case sm_torrent:
        {

        }
        break;
        case sm_night:
        {

        }
        break;
        case sm_system_off:
        default:
        {
            state_static_red_obj.get_led_states(cpu_cooler_new_states, led_strip_new_states, front_fans_new_states);
            state_static_red_obj.check_if_state_should_change();
        }
        break;        
    }

    // Update the actual LED data
    
}

void copy_led_data(CRGB *src, CRGB *dest, uint8_t len)
{
    for (uint8_t index = 0U; index < len; ++index)
    {
        dest[index].red = src[index].red;
        dest[index].green = src[index].green;
        dest[index].blue = src[index].blue;
    }
}

#if defined(DEBUG) && (DEBUG == 1)
void debug(void)
{
    while (true)
    {
        for (signed int i = 0; i < NUM_OF_LEDS; i++)
        {
            if (i - 1 >= 0)
                leds[i - 1] = CRGB(0, 0, 0);
            leds[i] = CRGB(255, 0, 0);
            FastLED.show();
            Serial.println(i);
            delay(1000);
        }
    }
}
#endif
