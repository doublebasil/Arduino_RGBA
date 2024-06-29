#include "fan_speed.hpp"

#include "common.hpp"

static int fan_speeds[NUM_OF_FAN_READINGS]; // Note: Each value is 0 to 1024 (10 bit)
static uint8_t fan_speeds_index = 0U;
static bool first_reading = true;

void check_fan_speed(void)
{
    int reading = analogRead(FAN_SPEED_MEASURE_PIN);
    if (first_reading == true)
    {
        for (uint8_t index = 0U; index < NUM_OF_FAN_READINGS; ++index)
        {
            fan_speeds[index] = reading;
        }
    }
    else
    {
        fan_speeds[fan_speeds_index] = reading;
        ++fan_speeds_index;
        if (fan_speeds_index == NUM_OF_FAN_READINGS)
        {
            fan_speeds_index = 0U;
        }
    }
}

uint16_t get_fan_speed(void)
{
    uint64_t average = 0U;
    for (uint8_t index = 0U; index < NUM_OF_FAN_READINGS; ++index)
    {
        average += fan_speeds[index];
    }
    average /= NUM_OF_FAN_READINGS;
    return (uint16_t) average;
}