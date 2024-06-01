#include "temperature_sensor.hpp"
#include <Adafruit_AHTX0.h>
#include "common.hpp"
#include "state_abstract.hpp"

static uint8_t cycles_until_reading = SYS_CYCLES_PER_TEMP_READING;

static bool sensor_initialised = false;

static Adafruit_AHTX0 aht;

static sensors_event_t humidity_sensor_evt;
static sensors_event_t temp_sensor_evt;

static uint8_t temperature_readings_index = 0;
static float temperature_readings[NUM_OF_TEMP_READINGS];

float get_temperature(void)
{
    if (sensor_initialised == false)
        return 0.0;
    
    float total = 0.0;
    for (uint8_t index = 0U; index < NUM_OF_TEMP_READINGS; ++index)
    {
        total += temperature_readings[index];
    }
    total /= (float) NUM_OF_TEMP_READINGS;
    return total;
}

bool is_temperature_sensor_initialised(void)
{
    return sensor_initialised;
}

void init_temperature_sensor(void)
{
    sensor_initialised = aht.begin();

    if (sensor_initialised == true)
    {
        // Take a temperature reading and use this to populate the temperature_readings array
        aht.getEvent(&humidity_sensor_evt, &temp_sensor_evt);
        for (uint8_t index = 0U; index < NUM_OF_TEMP_READINGS; ++index)
        {
            temperature_readings[index] = temp_sensor_evt.temperature;
        }
    }
}

void update_temperature_sensor(void)
{
    if (sensor_initialised == false)
        return;
    
    --cycles_until_reading;
    if (cycles_until_reading != 0)
        return;

    cycles_until_reading = SYS_CYCLES_PER_TEMP_READING;
    aht.getEvent(&humidity_sensor_evt, &temp_sensor_evt);
    temperature_readings[temperature_readings_index] = temp_sensor_evt.temperature;
    ++temperature_readings_index;
    if (temperature_readings_index == NUM_OF_TEMP_READINGS)
        temperature_readings_index = 0U;
}
