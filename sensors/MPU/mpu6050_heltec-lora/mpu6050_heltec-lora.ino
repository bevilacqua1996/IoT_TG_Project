#include<Wire.h>

#define Register_2D 0x6B
#define Register_X0 0x3C
#define Register_X1 0x3B
#define Register_Y0 0x3E
#define Register_Y1 0x3D
#define Register_Z0 0x40
#define Register_Z1 0x3F

const int MPU_ADDR = 0x68; // I2C address of the MPU-6050
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

int X0,X1,X_out;
int Y0,Y1,Y_out;
int Z1,Z0,Z_out;
double Xg,Yg,Zg;

void setup() {
   Serial.begin(115200);
   Wire.begin(4, 15, 100000); // sda, scl, clock speed
   Wire.beginTransmission(MPU_ADDR);
   Wire.write(0x6B);  // PWR_MGMT_1 register
   Wire.write(0);     // set to zero (wakes up the MPU−6050)
   Wire.endTransmission(true);
   Serial.println("Setup complete");
}

void loop() {
//   Wire.beginTransmission(MPU_ADDR);
//   Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
//   Wire.endTransmission(true);
//   Wire.beginTransmission(MPU_ADDR);
   AcZ = getZAcc(MPU_ADDR);
   //Wire.requestFrom(MPU_ADDR, 14, true); // request a total of 14 registers
//   Wire.requestFrom(MPU_ADDR, 6, true);
//   AcX = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
//   AcY = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
//   AcZ = Wire.read() << 8 | Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
//   Tmp = Wire.read() << 8 | Wire.read(); // 0x41 (TEMP_OUT_H) &  0x42 (TEMP_OUT_L)
//   GyX = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
//   GyY = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
//   GyZ = Wire.read() << 8 | Wire.read();
//   Serial.print(AcX); Serial.print(" , ");
//   Serial.print(AcY); Serial.print(" , ");
   Serial.print(AcZ); Serial.print(" , ");
//   Serial.print(GyX); Serial.print(" , ");
//   Serial.print(GyY); Serial.print(" , ");
//   Serial.print(GyZ); Serial.print("\n");
}

int getZAcc(int adxAddress)
{
  //------------------Z
  Wire.beginTransmission(adxAddress); // transmit to device
  //Wire.write(Register_Z0);
  Wire.write(Register_Z1);
  Wire.endTransmission();
  //Wire.requestFrom(adxAddress,2); 
  Wire.requestFrom(adxAddress,2,true);
  if(Wire.available()<=2)   
  {
//    Z0 = Wire.read();
//    Z1 = Wire.read(); 
//    Z1=Z1<<8;
//    Z_out=Z0+Z1;
      Z_out = Wire.read() << 8 | Wire.read();
  }
  //
  Serial.print("Acc z: ");
  Serial.println(Z_out);
  //return Zg;
  return Z_out;
}
