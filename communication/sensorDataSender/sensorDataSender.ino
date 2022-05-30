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

volatile unsigned int counter = 0;
String rssi = "RSSI --";
String packSize = "--";
String packet;

//const long DELAY_ = 60000;
const long DELAY_ = 2000;
unsigned long last_t_task1 = 0;

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

const TickType_t _1s = 1000 / portTICK_PERIOD_MS;
const TickType_t _1ms = 1 / portTICK_PERIOD_MS;
const TickType_t _100ms = 100 / portTICK_PERIOD_MS;
const TickType_t _5s = 5000 / portTICK_PERIOD_MS; 
TaskHandle_t Task1;
static String tmp="";

class SensorValues{
  public:
    SensorValues(unsigned short array_size, unsigned short factor, unsigned char code){
      this->array_size = array_size;
      this->values = new int[array_size];
      std::fill_n(this->values,array_size,-200);
      this->factor = factor;
      this->code = code;
    }
    
    void add_value(float value) volatile{
      if(!is_reading){
        if(this->index < this->array_size){
          is_writing = 1;
          this->values[index] = value*factor;
          this->index++;
          is_writing = 0;
        }
      }
    }
    
    float get_value_at_index(short index){
      return this->values[index];
    }

    String publish_values() volatile{
      String str_values = "";
      while(is_writing){
        //vTaskDelay( _1ms );
        //taskYIELD();
      }
      is_reading = 1;
      for(int i=0;i<this->array_size;i++){
        if(this->values[i]==-200)
          break;
        //str_values += String(this->values[i]/float(this->factor)) + " ,";
//        String str_val = String(this->code) + " : " + String(this->values[i]/float(this->factor));
//        str_values += str_val + ",";
        str_values += String(this->code) + " : " + String(this->values[i]/float(this->factor)) + ",";
        //this->send_packet(str_val);
        this->values[i]=-200;
        //taskYIELD();
      }
      this->index = 0;
      is_reading = 0;
      return str_values;
      //Serial.println(str_values);
    }

    void send_packet(String message) volatile{
      // send packet
      LoRa.beginPacket();
      LoRa.setTxPower(14,RF_PACONFIG_PASELECT_PABOOST);
      LoRa.print(message);
      LoRa.endPacket();
      counter++;
    }
    
    volatile bool is_reading=1;
    volatile bool is_writing=0;
  private:
    volatile unsigned short index;
    volatile int *values;
    volatile unsigned short array_size;
    volatile unsigned short factor;
    unsigned char code;
};

volatile SensorValues Rotations = SensorValues(20,1000,ROTATIONS_CODE);
volatile SensorValues Temperatures = SensorValues(20,100,TEMPERATURE_CODE);
volatile SensorValues Accx = SensorValues(20,1000,ACC_X_CODE);
volatile SensorValues Accy = SensorValues(20,1000,ACC_Y_CODE);
volatile SensorValues Accz = SensorValues(20,1000,ACC_Z_CODE);
volatile SensorValues Voltages = SensorValues(20,1000,VOLTAGE_CODE);

void setup()
{
  attachInterrupt(digitalPinToInterrupt(OPTICAL_PIN), get_delta, FALLING);

  xTaskCreatePinnedToCore(
    Task1code, /* Function to implement the task */
    "Task1", /* Name of the task */
    //24000,  /* Stack size in words */
    32768,  /* Stack size in words */
    NULL,  /* Task input parameter */
    5,  /* Priority of the task */
    &Task1,  /* Task handle. */
    0); /* Core where the task should run */
  
  // configure for automatic base-line restoration and continuous scan mode:
  readRms.begin(VoltRange, RMS_WINDOW, ADC_12BIT, BLR_ON, CNT_SCAN);
  
  // configure for no baseline restauration and single scan mode:
  readRms.start(); //start measuring
  
  nextLoop = micros() + LPERIOD; // Set the loop timer variable for the next loop interval.

//  setup_imu();

  DS18B20.begin();    // initialize the DS18B20 sensor
  
   //WIFI Kit series V1 not support Vext control
  Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.Heltec.Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);
//  Serial.begin(115200);
 
  Heltec.display->init();
  Heltec.display->flipScreenVertically();  
  Heltec.display->setFont(ArialMT_Plain_10);
  //logo();
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
  vTaskDelay( _100ms );
//  vTaskDelay( _1s );
//  //publish_all_values();
  UBaseType_t uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL );
  String mensagem = "Core:" + String((int)xPortGetCoreID()) + " --> Stack used: " + String((uint32_t)uxHighWaterMark);
  Serial.println(mensagem);
}

void Task1code( void * parameter) {
  Serial.print("Core: ");Serial.println(xPortGetCoreID());
  for(;;) {
    tmp += Rotations.publish_values();
    tmp += Temperatures.publish_values();
    tmp += Accx.publish_values();
    tmp += Accy.publish_values();
    tmp += Accz.publish_values();
    tmp += Voltages.publish_values();
    sendPacket(tmp);
//    sendPacket(String(TEMPERATURE_CODE) + " : " + "-100.0");
    Serial.println(tmp);
    tmp="";
    UBaseType_t uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL );
    String mensagem = "Core:" + String((int)xPortGetCoreID()) + " --> Stack used: " + String((uint32_t)uxHighWaterMark);
    Serial.println(mensagem);
    vTaskDelay( _5s );
  }
}

void temperatureSensor() {
  DS18B20.requestTemperatures();       // send the command to get temperatures
  tempC = DS18B20.getTempCByIndex(0);  // read temperature in °C
//  sendPacket(String(TEMPERATURE_CODE) + " : " + String(tempC));
//  delay(DELAY_);
  Temperatures.add_value(tempC);
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
//  sendPacket(String(ACC_X_CODE) + " : " + String(accX));
//  delay(DELAY_/3);
  int accY = getYAcc(ADXAddress);
//  sendPacket(String(ACC_Y_CODE) + " : " + String(accY));
//  delay(DELAY_/3);
  int accZ = getZAcc(ADXAddress);
//  sendPacket(String(ACC_Z_CODE) + " : " + String(accZ));
//  delay(DELAY_/3);
  Accx.add_value(accX);
  Accy.add_value(accX);
  Accz.add_value(accX);
}

void opticalSensor() {
  long starttime = millis();
  long endtime = starttime;
  //int rotacoes = opticalSensorProcess(DELAY_, OPTICAL_PIN);
  int rotations=0;
  while((endtime-starttime)<=DELAY_){
    if(dt_vel){
      rotations = 60000000/dt_vel;
      //Serial.println(rotacoes);
    }
    endtime = millis();
  }
  //sendPacket(String(ROTATIONS_CODE) + " : " + String(rotacoes));
  Rotations.add_value(rotations);
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
  //sendPacket(String(VOLTAGE_CODE) + " : " + String(540.0*readRms.rmsVal,2));
  Voltages.add_value(540.0*readRms.rmsVal);
}

void sendPacket(String message) {
  // send packet
  LoRa.beginPacket();
  
  LoRa.setTxPower(14,RF_PACONFIG_PASELECT_PABOOST);
  LoRa.print(message);
  LoRa.endPacket(true);
  counter++;
}

void publish_all_values(){
  Rotations.publish_values();
  delay(DELAY_);
  Temperatures.publish_values();
  delay(DELAY_);
  Accx.publish_values();
  delay(DELAY_);
  Accy.publish_values();
  delay(DELAY_);
  Accz.publish_values();
  delay(DELAY_);
  Voltages.publish_values();
  delay(DELAY_);
}

void logo()
{
  Heltec.display->clear();
  Heltec.display->drawXbm(0,5,logo_width,logo_height,logo_bits);
  Heltec.display->display();
}
