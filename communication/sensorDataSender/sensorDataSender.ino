/*
 * Camada de comunicação LoRa e API
 * 
 * Função: Sender, envio de dados para LoRa receiver
*/

#include <heltec.h>
#include "images.h"
#include <opticalSensorReading.h>
#include <TrueRMSNew.h>
#include <acc.h>
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ROTATIONS_CODE 1
#define VOLTAGE_CODE 2
#define ACC_X_CODE 3
#define ACC_Y_CODE 4
#define ACC_Z_CODE 5
#define TEMPERATURE_CODE 6

#define BAND    915E6  //you can set band here directly,e.g. 868E6,915E6

#define LPERIOD 100    // loop period time in us. In this case 100 us
#define VOLTAGE_PIN 36     // define the used ADC input channel
#define RMS_WINDOW 5000   // rms window of 1667 samples, means 10 periods @60Hz

#define Register_2D 0x2D

#define TEMPERATURE_PIN 32 // ESP32 pin GIOP33 connected to DS18B20 sensor's DQ pin

unsigned int counter = 0;
String rssi = "RSSI --";
String packSize = "--";
String packet;

//const long DELAY_ = 60000;
const long DELAY_ = 2000;

// Optical
const int OPTICAL_PIN = 23; // possible digital Input for LoRa32
unsigned long dt_vel=0;
unsigned long timer_aux=0;
int rotation=0;

// Temperature
OneWire oneWire(TEMPERATURE_PIN);
DallasTemperature DS18B20(&oneWire);
float tempC; // temperature in Celsius

// Voltage
Rms readRms; // create an instance of Rms.
unsigned long nextLoop;
int adcVal;
int cnt=0;
float VoltRange = 3.30; // The full scale value is set to 5.00 Volts but can be changed when using an
                        // input scaling circuit in front of the ADC.
unsigned long last_time = 0;

// MPU6050
//int ADXAddress = 0x53;  // the default 7-bit slave address
int ADXAddress = 0x68;


void setup()
{
  attachInterrupt(digitalPinToInterrupt(OPTICAL_PIN), get_delta, FALLING);
  
  // configure for automatic base-line restoration and continuous scan mode:
  readRms.begin(VoltRange, RMS_WINDOW, ADC_12BIT, BLR_ON, CNT_SCAN);
  
  // configure for no baseline restauration and single scan mode:
  readRms.start(); //start measuring
  
  nextLoop = micros() + LPERIOD; // Set the loop timer variable for the next loop interval.

  setup_imu();

  DS18B20.begin();    // initialize the DS18B20 sensor
  
   //WIFI Kit series V1 not support Vext control
  Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.Heltec.Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);
 
  Heltec.display->init();
  Heltec.display->flipScreenVertically();  
  Heltec.display->setFont(ArialMT_Plain_10);
  logo();
  delay(1500);
  Heltec.display->clear();
  
  Heltec.display->drawString(0, 0, "Heltec.LoRa Initial success!");
  Heltec.display->display();
  delay(1000);

  //pinMode(OPTICAL_PIN, INPUT);
  
}

void loop()
{
  Heltec.display->clear();
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->setFont(ArialMT_Plain_10);
  
  Heltec.display->drawString(0, 0, "Sending packet: ");
  Heltec.display->drawString(90, 0, String(counter));
  Heltec.display->display();
  
  voltageSensor();
  opticalSensor();
  accSensor();
  temperatureSensor();
}

void temperatureSensor() {
  DS18B20.requestTemperatures();       // send the command to get temperatures
  tempC = DS18B20.getTempCByIndex(0);  // read temperature in °C
  sendPacket(String(TEMPERATURE_CODE) + " : " + String(tempC));
  delay(DELAY_);

  String mess = "Temperatura [ºC]: " + String(tempC);
  Serial.println(mess);
}

void setup_imu(){
//  Wire.begin();                
//  // enable to measute g data
//  Wire.beginTransmission(ADXAddress);
//  Wire.write(Register_2D);
//  Wire.write(8);                //measuring enable
//  Wire.endTransmission();     // stop transmitting
  Wire.begin(4, 15, 100000); // sda, scl, clock speed
  Wire.beginTransmission(ADXAddress);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU−6050)
  Wire.endTransmission(true);
  Serial.println("Setup complete");
}

void accSensor() {
  int accX = getXAcc(ADXAddress);
  sendPacket(String(ACC_X_CODE) + " : " + String(accX));
  delay(DELAY_/3);
  int accY = getYAcc(ADXAddress);
  sendPacket(String(ACC_Y_CODE) + " : " + String(accY));
  delay(DELAY_/3);
  int accZ = getZAcc(ADXAddress);
  sendPacket(String(ACC_Z_CODE) + " : " + String(accZ));
  delay(DELAY_/3);

  String mess = "Aceleração: x=" + String(accX) + ", y=" + String(accY) + ", z=" + String(accZ);
  Serial.println(mess);
}

void opticalSensor() {
  long starttime = millis();
  long endtime = starttime;
  //int rotacoes = opticalSensorProcess(DELAY_, OPTICAL_PIN);
  int rotacoes=0;
  while((endtime-starttime)<=DELAY_){
    if(dt_vel){
      rotacoes = 60000000/dt_vel;
      //Serial.println(rotacoes);
    }
    endtime = millis();
  }
  sendPacket(String(ROTATIONS_CODE) + " : " + String(rotacoes));
  
  String mess = "Rotações [rpm]: " + String(rotacoes);
  Serial.println(mess);
}

void get_delta(){   // interrupt
    dt_vel = micros() - timer_aux;                                       // Calcula delta de tempo entre ultimo pulso e instante atual
    timer_aux = micros();                                                // Atualiza instante do ultimo pulso
}

void voltageSensor() {
  long starttime = millis();
  long endtime = starttime;
  while((endtime-starttime)<=DELAY_) {
    adcVal = analogRead(VOLTAGE_PIN); // read the ADC.
    while(nextLoop > micros());  // wait until the end of the loop time interval
    nextLoop += LPERIOD;
    readRms.update(adcVal); // update
    endtime = millis();
  }
  readRms.publish();
  sendPacket(String(VOLTAGE_CODE) + " : " + String(540.0*readRms.rmsVal,2));
  
  String mess = "Tensão [V]: " + String(540.0*readRms.rmsVal,2);
  Serial.println(mess);
}

void sendPacket(String message) {
  // send packet
  LoRa.beginPacket();
  
  LoRa.setTxPower(14,RF_PACONFIG_PASELECT_PABOOST);
  LoRa.print(message);
  LoRa.endPacket();
  counter++;
}

void logo()
{
  Heltec.display->clear();
  Heltec.display->drawXbm(0,5,logo_width,logo_height,logo_bits);
  Heltec.display->display();
}
