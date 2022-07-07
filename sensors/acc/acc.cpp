#include "acc.h"

int X0,X1,X_out;
int Y0,Y1,Y_out;
int Z1,Z0,Z_out;
double Xg,Yg,Zg;

int getXAcc(int adxAddress)
{
  //--------------X
 /* Wire.beginTransmission(adxAddress); // transmit to device
  Wire.write(Register_X0);
  Wire.write(Register_X1);
  Wire.endTransmission();
  Wire.requestFrom(adxAddress,2); 
  if(Wire.available()<=2)   
  {
    X0 = Wire.read();
    X1 = Wire.read(); 
    X1=X1<<8;
    X_out=X0+X1;   
  }
  Xg=X_out/256.0; */
  
  Wire.beginTransmission(adxAddress); // transmit to device
  Wire.write(Register_X1);
  Wire.endTransmission();
  Wire.requestFrom(adxAddress,2,true);
  if(Wire.available()<=2)   
  {
      X_out = Wire.read() << 8 | Wire.read();
  }

  return X_out;
}

int getYAcc(int adxAddress)
{
  //------------------Y
  /* Wire.beginTransmission(adxAddress); // transmit to device
  Wire.write(Register_Y0);
  Wire.write(Register_Y1);
  Wire.endTransmission();
  Wire.requestFrom(adxAddress,2); 
  if(Wire.available()<=2)   
  {
    Y0 = Wire.read();
    Y1 = Wire.read(); 
    Y1=Y1<<8;
    Y_out=Y0+Y1;
  }
  Yg=Y_out/256.0; */
  
  Wire.beginTransmission(adxAddress); // transmit to device
  Wire.write(Register_Y1);
  Wire.endTransmission();
  Wire.requestFrom(adxAddress,2,true);
  if(Wire.available()<=2)   
  {
      Y_out = Wire.read() << 8 | Wire.read();
  }

  return Y_out;
}

int getZAcc(int adxAddress)
{
  //------------------Z
/*   Wire.beginTransmission(adxAddress); // transmit to device
  Wire.write(Register_Z0);
  Wire.write(Register_Z1);
  Wire.endTransmission();
  Wire.requestFrom(adxAddress,2); 
  if(Wire.available()<=2)   
  {
    Z0 = Wire.read();
    Z1 = Wire.read(); 
    Z1=Z1<<8;
    Z_out=Z0+Z1;
  }
  //
  Zg=Z_out/256.0; */
  
  Wire.beginTransmission(adxAddress); // transmit to device
  Wire.write(Register_Z1);
  Wire.endTransmission();
  Wire.requestFrom(adxAddress,2,true);
  if(Wire.available()<=2)   
  {
      Z_out = Wire.read() << 8 | Wire.read();
  }
  
  return Z_out;
}
