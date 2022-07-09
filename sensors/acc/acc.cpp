#include "acc.h"
#include "FFT_C.h"

MPU6050 accelgyro;
unsigned int accel_max = pow(2,14)/pow(2,accelgyro.getFullScaleAccelRange());
//gyro_max = 131/pow(2,accelgyro.getFullScaleGyroRange());
unsigned long t_cycle_us = 1000000/sample_freq;

unsigned long t;

int16_t getXAccRaw()
{
  return accelgyro.getAccelerationX();
}

int16_t getYAccRaw()
{
  return accelgyro.getAccelerationY();
}

int16_t getZAccRaw()
{
  return accelgyro.getAccelerationZ();
}

float getXAcc()
{
  return getXAccRaw() * gravity / accel_max;
}

float getYAcc()
{
  return getYAccRaw() * gravity / accel_max;
}

float getZAcc()
{
  return getZAccRaw() * gravity / accel_max;
}

void getAcc(int16_t ax, int16_t ay, int16_t az){
	accelgyro.getAcceleration(&ax, &ay, &az);
}

void calibrateAcc(){
	accelgyro.CalibrateAccel(10);
	Serial.println("\nat 1000 Readings");
	accelgyro.PrintActiveOffsets();
}

void fft_scan(){
	for(int i=0; i<FFT_SIZE;i++){
	raw[n] = getXAccRaw();
	n++;
    if(n==FFT_SIZE){
      FFT(raw,spectr);
      calc_fft();
      //print_fft_spectre();
      zero_fft_spectre();
      n=0;
    }
    while(micros()-t < t_cycle_us){vTaskDelay(0);}
    t=micros();
	}
}

void calc_fft(){
  int low_raw = 0;
  int high_raw = 0;
  for(int j=0;j<1;j++){
    for (int i = 0; i < FFT_SIZE / 2; i++) {
      spectr[i] = (spectr[i] * (i + 2)) >> 1;
      if (i < 2) low_raw += spectr[i];
      else high_raw += spectr[i];
    }
  }
}

void zero_fft_spectre(){
  for(int i=0;i<FFT_SIZE;i++)
    raw[i] = 0;
}

void print_fft_spectre(){
  for (int i = 0; i < FFT_SIZE; i++){
    Serial.print(sample_freq*i/float(FFT_SIZE)); Serial.print("\t");
    Serial.println(spectr[i]);
  }
}

float get_amplitude_at_freq(float freq){
	int k = freq / (sample_freq/float(FFT_SIZE));
	return spectr[k] * gravity / accel_max / FFT_SIZE;
}

float get_amplitude_at_2fl(){
	return get_amplitude_at_freq(line_freq);
}

float get_pass_pole_freq(){
	return (line_freq - nom_freq) * num_of_poles;
}

float* get_amplitude_at_lateral_band(float fcentral, float delta){
	return new float[2]{get_amplitude_at_freq(fcentral - delta), get_amplitude_at_freq(fcentral + delta)};
}

