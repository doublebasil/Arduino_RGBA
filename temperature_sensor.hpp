#ifndef TEMPERATURE_SENSOR_HPP
#define TEMPERATURE_SENSOR_HPP

#include <Arduino.h>

float get_temperature(void);

bool is_temperature_sensor_initialised(void);

void init_temperature_sensor(void);

void update_temperature_sensor(void);

#endif // ndef TEMPERATURE_SENSOR_HPP
