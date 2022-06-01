#ifndef SENSOR_DATA_JSON_H
#define SENSOR_DATA_JSON_H
#include <Arduino.h>
#include <ArduinoJson.h>
#include "support.h"
//#include <sstream>
#include <Vector.h>
#define ROTATIONS_CODE 1
#define VOLTAGE_CODE 2
#define ACC_X_CODE 3
#define ACC_Y_CODE 4
#define ACC_Z_CODE 5
#define TEMPERATURE_CODE 6
String buildJSON(String packet);
long getTimestamp();
int getRpm();
double getVoltage();
#endif
