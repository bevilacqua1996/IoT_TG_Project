#ifndef ACC_H
#define ACC_H

#include <Arduino.h>
#include <Wire.h>
#define Register_2D 0x6B
#define Register_X0 0x3C
#define Register_X1 0x3B
#define Register_Y0 0x3E
#define Register_Y1 0x3D
#define Register_Z0 0x40
#define Register_Z1 0x3F
int getXAcc(int adxAddress);
int getYAcc(int adxAddress);
int getZAcc(int adxAddress);
#endif
