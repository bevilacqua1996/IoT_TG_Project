#ifndef ACC_H
#define ACC_H

#define FFT_SIZE 512  // 32/64/128/256/512
#define sample_freq 1000 
#define Register_2D 0x6B
#define Register_X0 0x3C
#define Register_X1 0x3B
#define Register_Y0 0x3E
#define Register_Y1 0x3D
#define Register_Z0 0x40
#define Register_Z1 0x3F

#include <Arduino.h>
#include <Wire.h>
#include <MPU6050.h>
//#include "FFT_C.h"

const float gravity = 9.80665;
//const float acc_const = gravity / accel_max;
const float line_freq = 60;
const float nom_freq = 3500/60;
const int num_of_poles = 2;

static int n;
static int raw[FFT_SIZE];
static int spectr[FFT_SIZE];

float getXAcc();
float getYAcc();
float getZAcc();
void calibrateAcc();
void fft_scan();
void calc_fft();
void zero_fft_spectre();
void print_fft_spectre();
float get_amplitude_at_freq(float freq);
float get_amplitude_at_2fl();
float get_pass_pole_freq();
float* get_amplitude_at_lateral_band(float fcentral, float delta);

#endif
