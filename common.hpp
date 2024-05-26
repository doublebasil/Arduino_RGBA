#ifndef COMMON_HPP
#define COMMON_HPP

#define POWER_BUTTON_ADC_PIN        ( A0 )  // For reading the state of the power button LED
#define POWER_BUTTON_ADC_THRESHOLD  ( 300 ) // Used with ADC to determine if the power button LED is on

#define RESET_BUTTON_INPUT_PIN      ( A1 )  // Using the pc case reset button as a button for the Arduino

#define CPU_COOLER_DATA_PIN         ( 12 )
#define LED_STRIP_DATA_PIN          ( 11 )
#define FRONT_FANS_DATA_PIN         ( 10 )

#define UPDATE_PERIOD_MS            ( 20 )
#define TRANSITION_CYCLES           ( 1000 / UPDATE_PERIOD_MS )

#define CPU_COOLER_NUM_LEDS         ( 24 )
#define CPU_COOLER_INNER_NUM_LEDS   ( 8 )
#define CPU_COOLER_OUTER_NUM_LEDS   ( 16 )
#define LED_STRIP_NUM_LEDS          ( 7 )
#define FRONT_FANS_NUM_LEDS         ( 12 )

#define DEFAULT_MODE                ( sm_torrent ) // I want to get rid of this at some point

#include "FastLED.h"

typedef enum
{
    sm_system_off,
    sm_static_red,
    sm_thermal,
    sm_torrent,
    sm_lava_lamp,
    sm_night,
} system_state_t;

typedef enum
{
    pc_on,
    pc_off,
} pc_state_t;

pc_state_t get_pc_state(void);
void copy_led_data(CRGB src[], CRGB dest[], uint8_t len);

#endif // ndef COMMON_HPP
