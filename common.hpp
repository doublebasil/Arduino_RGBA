#ifndef COMMON_HPP
#define COMMON_HPP

#define POWER_BUTTON_ADC_PIN        ( A0 )  // For reading the state of the power button LED
#define POWER_BUTTON_ADC_THRESHOLD  ( 300 ) // Used with ADC to determine if the power button LED is on

#define RESET_BUTTON_INPUT_PIN      ( A1 )  // Using the pc case reset button as a button for the Arduino

#define CPU_COOLER_DATA_PIN         ( 12 )
#define LED_STRIP_DATA_PIN          ( 11 )
#define FRONT_FANS_DATA_PIN         ( 10 )

#define UPDATE_PERIOD_MS            ( 25 )
#define TRANSITION_TIME_MS          ( 1000 )
#define TRANSITION_CYCLES           ( TRANSITION_TIME_MS / UPDATE_PERIOD_MS )

#define FAN_SPEED_MEASURE_PIN       ( A5 )
#define NUM_OF_FAN_READINGS         ( 3 )

#define FRONT_FANS_NUM_LEDS         ( 5 )

#define DEFAULT_MODE                ( sm_thermal )

#include "FastLED.h"

typedef enum
{
    sm_system_off,
    sm_static,
    sm_thermal,
    sm_torrent,
} system_state_t;

typedef enum
{
    pc_on,
    pc_off,
} pc_state_t;

pc_state_t get_pc_state(void);
void copy_led_data(CRGB src[], CRGB dest[], uint8_t len);

#endif // ndef COMMON_HPP
