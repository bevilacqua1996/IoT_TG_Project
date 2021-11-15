#ifndef SENSOR_DATA_JSON_H
#define SENSOR_DATA_JSON_H
#include <Arduino.h>
#include <ArduinoJson.h>
#include "support.h"
#define ROTATIONS_CODE 1
#define VOLTAGE_CODE 2
String buildJSON(String packet);
long getTimestamp();
int getRpm();
double getVoltage();
#endif
