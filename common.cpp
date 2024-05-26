#include <Arduino.h>
#include "common.hpp"

pc_state_t get_pc_state(void)
{
    int analog_reading = analogRead(ANALOG_READ_PIN);
    if (analog_reading < ANALOG_THRESHOLD)
        return pc_on;
    else
        return pc_off;
}
