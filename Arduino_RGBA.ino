#include "FastLED.h"

#define ANALOG_READ_PIN  ( A0 )
#define DIGITAL_OUT_PIN  ( 11 )

#define ANALOG_THRESHOLD ( 300 )

#define UPDATE_PERIOD_MS ( 250 )

typedef enum
{
    pc_on,
    pc_off,
} pc_state_t;

pc_state_t get_pc_state(void);

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
    unsigned long loop_start_time =  millis();

    pc_state_t pc_state = get_pc_state();

    // This bit is just temporary
    if (pc_state == pc_on)
        digitalWrite(LED_BUILTIN, HIGH);
    else
        digitalWrite(LED_BUILTIN, LOW);

    unsigned long loop_duration = millis() - loop_start_time;
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
