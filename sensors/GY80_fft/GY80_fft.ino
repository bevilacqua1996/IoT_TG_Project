#define FFT_SIZE 512  // 32/64/128/256/512
#define sample_freq 1000

#include "acc.h"
#include "Wire.h"
#include <FFT_C.h>

//int16_t ax, ay, az;
float accx, accy, accz;
int16_t gx, gy, gz;
float acc_real[3];
//float gyro_real[3];

const float gravity = 9.8066;
int accel_max;
float gyro_max, const_acel, const_gyro;
unsigned long t;
unsigned long t_cycle_us = 1000000/sample_freq;
unsigned long t_last_cycle_us = 0;

int n=0;
static int raw[FFT_SIZE];
static int spectr[FFT_SIZE];

void setup() {
    // join I2C bus (I2Cdev library doesn't do this automatically)
    Wire.begin();
    Serial.begin(38400);
    calc_consts();
}

void loop() {
    // ---- read raw accel/gyro measurements from device ---
    ax = acc.getXAcc();
    update_fft(ax);
    if(n==FFT_SIZE){
      FFT(raw,spectr);
      calc_fft();
      print_fft_spectre();
      zero_fft_spectre();
      n=0;
//      display_time();
//      t_last_cycle_us = micros();
    }
    while(micros()-t < 1000);
    t=micros();
}

void display_readings(){
   // display tab-separated accel/gyro x/y/z values
  //    Serial.print("a/g:\t");
      Serial.print(acc_real[0]); Serial.print("\t");
      Serial.print(acc_real[1]); Serial.print("\t");
      Serial.print(acc_real[2]);
  //    Serial.print(az_real); Serial.print("\t");
  //    Serial.print(gx_real); Serial.print("\t");
  //    Serial.print(gy_real); Serial.print("\t");
  //    Serial.print(gz_real);
      Serial.println();
}

void display_time(){
  Serial.println(micros()-t_last_cycle_us);
}

void calc_accel(float *a){
  a[0] = ax*const_acel;
  a[1] = ay*const_acel;
  a[2] = az*const_acel;
}

void calc_gyro(float *g){
  g[0] = gx*const_gyro;
  g[1] = gy*const_gyro;
  g[2] = gz*const_gyro;
}

void calc_consts(){
  accel_max = pow(2,14)/pow(2,accelgyro.getFullScaleAccelRange());
  gyro_max = 131/pow(2,accelgyro.getFullScaleGyroRange());
  const_acel = gravity/accel_max;
  const_gyro = 1/gyro_max;
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

void print_fft_spectre(){
  for (int i = 0; i < FFT_SIZE; i++){
    Serial.print(sample_freq*i/float(FFT_SIZE)); Serial.print("\t");
    Serial.println(spectr[i]);
  }
}

void update_fft(int new_val){ 
  raw[n] = new_val;
  n++;
}

void zero_fft_spectre(){
  for(int i=0;i<FFT_SIZE;i++)
    raw[i] = 0;
}
