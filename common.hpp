#ifndef COMMON_HPP
#define COMMON_HPP

#define ANALOG_READ_PIN           ( A0 )
#define ANALOG_THRESHOLD          ( 300 )

#define CPU_COOLER_DATA_PIN       ( 12 )
#define LED_STRIP_DATA_PIN        ( 11 )
#define FRONT_FANS_DATA_PIN       ( 10 )

#define UPDATE_PERIOD_MS          ( 20 )
#define TRANSITION_CYCLES         ( 1000 / UPDATE_PERIOD_MS )

#define CPU_COOLER_NUM_LEDS       ( 24 )
#define CPU_COOLER_INNER_NUM_LEDS ( 8 )
#define CPU_COOLER_OUTER_NUM_LEDS ( 16 )
#define LED_STRIP_NUM_LEDS        ( 7 )
#define FRONT_FANS_NUM_LEDS       ( 12 )

#define DEFAULT_MODE              ( sm_static_red ) // I want to get rid of this at some point

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

#endif // ndef COMMON_HPP
