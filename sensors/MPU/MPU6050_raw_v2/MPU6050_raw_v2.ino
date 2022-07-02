// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
//#include "I2Cdev.h"
#include "MPU6050.h"

// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
//#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
//    #include "Wire.h"
//#endif
#include "Wire.h"

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
unsigned int t;

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
     //Serial.println("\n\n Any of the above offsets will work nice \n\n Lets proof the PID tuning using another method:"); 
}

void setup() {
    // join I2C bus (I2Cdev library doesn't do this automatically)

    Wire.begin(4,15,100000);
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
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    //accelgyro.getAcceleration(&ax, &ay, &az);
    //accelgyro.getRotation(&gx, &gy, &gz);
    
    //----- calculating real values ------
    calc_accel(acc_real);
    //calc_gyro(gyro_real);

    // ---------- printing values -------
    display(); 
}

void display(){
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
  //    //Serial.println(micros()-t);
  //    //t=micros();
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
