#define FFT_SIZE 512  // 32/64/128/256/512
#define sample_freq 1000

#include "MPU6050.h"
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif
#include <FFT_C.h>
 

// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for InvenSense evaluation board)
// AD0 high = 0x69
MPU6050 accelgyro;
//MPU6050 accelgyro(0x69); // <-- use for AD0 high

int16_t ax, ay, az;
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
//int sample_freq=1000;

void Calibrate(){
        accelgyro.CalibrateAccel(6);
        accelgyro.CalibrateGyro(6);
        Serial.println("\nat 600 Readings");
        accelgyro.PrintActiveOffsets();
        Serial.println();
        accelgyro.CalibrateAccel(1);
        accelgyro.CalibrateGyro(1);
        Serial.println("700 Total Readings");
        accelgyro.PrintActiveOffsets();
        Serial.println();
        accelgyro.CalibrateAccel(1);
        accelgyro.CalibrateGyro(1);
        Serial.println("800 Total Readings");
        accelgyro.PrintActiveOffsets();
        Serial.println();
        accelgyro.CalibrateAccel(1);
        accelgyro.CalibrateGyro(1);
        Serial.println("900 Total Readings");
        accelgyro.PrintActiveOffsets();
        Serial.println();    
        accelgyro.CalibrateAccel(1);
        accelgyro.CalibrateGyro(1);
        Serial.println("1000 Total Readings");
        accelgyro.PrintActiveOffsets();
}

void setup() {
    // join I2C bus (I2Cdev library doesn't do this automatically)
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif

    // initialize serial communication
    // (38400 chosen because it works as well at 8MHz as it does at 16MHz, but
    // it's really up to you depending on your project)
    Serial.begin(38400);

    // initialize device
    Serial.println("Initializing I2C devices...");
    accelgyro.initialize();

    // verify connection
    Serial.println("Testing device connections...");
    Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

    Calibrate();
    calc_consts();
//    Serial.println(const_acel,7);
//    Serial.println(const_gyro,5);
}

void loop() {
    // ---- read raw accel/gyro measurements from device ---
    //accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    accelgyro.getAcceleration(&ax, &ay, &az);
    //accelgyro.getRotation(&gx, &gy, &gz);
    
    //----- calculating real values ------
    //calc_accel(acc_real);
    //calc_gyro(gyro_real);

    // ---------- printing values -------
    //display_readings();

    // ------- fft ----------------
    //update_fft(acc_real[0]);
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

void set_offsets(int ax, int ay, int az, int gx, int gy, int gz){
//  use the code below to change accel/gyro offset values
//  Teste: OFFSETS     -951,   -6003,    1022,     -52,      42,    -105
  accelgyro.setXAccelOffset(ax);
  accelgyro.setYAccelOffset(ay);
  accelgyro.setZAccelOffset(az);
  accelgyro.setXGyroOffset(gx);
  accelgyro.setYGyroOffset(gy);
  accelgyro.setZGyroOffset(gz);
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
