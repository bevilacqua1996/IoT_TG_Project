
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
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <WiFi.h>
#include <WiFiUDP.h>

//#define ROTATIONS_CODE 1
//#define VOLTAGE_CODE 2
//#define ACC_X_CODE 3
//#define ACC_Y_CODE 4
//#define ACC_Z_CODE 5
//#define TEMPERATURE_CODE 6

#define TIME_CODE 0
#define ROTATIONS_CODE 1
#define VOLTAGE_CODE 2
#define ACC_X_CODE 3
#define ACC_Y_CODE 4
#define ACC_Z_CODE 5
#define TEMPERATURE_CODE 6
#define FACTORS_CODE 'F'

#define BAND    915E6  //you can set band here directly,e.g. 868E6,915E6

#define LPERIOD 100    // loop period time in us. In this case 100 us
#define VOLTAGE_PIN 36     // define the used ADC input channel
#define RMS_WINDOW 5000   // rms window of 1667 samples, means 10 periods @60Hz

#define Register_2D 0x2D

#define TEMPERATURE_PIN 32 // ESP32 pin GIOP33 connected to DS18B20 sensor's DQ pin

#define ARRAY_SIZE 15

volatile unsigned int counter = 0;
String rssi = "RSSI --";
String packSize = "--";
String packet;
int pkg_id=0;

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 3600;
extern const char* ssid;
extern const char* password;

//const long DELAY_ = 60000;
const long DELAY_ = 200;
const long VOLT_LOOP_TIME_ = 500;
const long OPT_LOOP_TIME_ = 500;
unsigned long last_t_task1 = 0;

// Optical
const int OPTICAL_PIN = 23; // possible digital Input for LoRa32
volatile unsigned long dt_vel=0;
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
const TickType_t _10ms = 10 / portTICK_PERIOD_MS;
const TickType_t _100ms = 100 / portTICK_PERIOD_MS;
const TickType_t _5s = 5000 / portTICK_PERIOD_MS; 
const TickType_t _10s = 10000 / portTICK_PERIOD_MS; 
TaskHandle_t Task1;
static String tmp="";

class SensorValues{
  public:
    SensorValues(unsigned short array_size, unsigned short factor, short code){
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
    
    float get_value_at_index(short index) volatile{
      return this->values[index];
    }

    short get_factor() volatile{
      return this->factor;
    }

    void clear_array() volatile{
      std::fill_n(values,array_size,-200);
      index=0;
    }

    String publish_values() volatile{
      String str_values = "";
      while(is_writing){
        vTaskDelay( _1ms );
        //taskYIELD();
      }
      is_reading = 1;
      DynamicJsonDocument msg(1024);
      msg["code"] = this->code;
      msg["factor"] = this->factor;
      msg["ts"] = time(NULL);
      JsonArray var_data = msg.createNestedArray("data");
      for(int i=0;i<this->array_size;i++){
        if(this->values[i]==-200)
          break;
        var_data.add(this->values[i]);
        this->values[i]=-200;
      }
      this->index = 0;

      String json;
      serializeJson(msg, json);
      send_packet(json);
      Serial.println(json);

      Serial.print("Memory usage: "); Serial.println(msg.memoryUsage());
      Serial.print("Sensor Json size: ");Serial.println(json.length());
      
      is_reading = 0;
      return json;
      //Serial.println(str_values);
    }

    void send_packet(String message) volatile{
      // send packet
      LoRa.beginPacket();
      LoRa.setTxPower(14,RF_PACONFIG_PASELECT_PABOOST);
      LoRa.print(message);
      LoRa.endPacket(true);
//      counter++;
    }
    
    volatile bool is_reading=0;
    volatile bool is_writing=1;
  private:
    volatile unsigned short index;
    volatile int *values;
    volatile unsigned short array_size;
    volatile unsigned short factor;
    char code;
};

volatile SensorValues Timestamps = SensorValues(ARRAY_SIZE,1,TIME_CODE);
volatile SensorValues Rotations = SensorValues(ARRAY_SIZE,1000,ROTATIONS_CODE);
volatile SensorValues Temperatures = SensorValues(ARRAY_SIZE,100,TEMPERATURE_CODE);
volatile SensorValues Accx = SensorValues(ARRAY_SIZE,1000,ACC_X_CODE);
volatile SensorValues Accy = SensorValues(ARRAY_SIZE,1000,ACC_Y_CODE);
volatile SensorValues Accz = SensorValues(ARRAY_SIZE,1000,ACC_Z_CODE);
volatile SensorValues Voltages = SensorValues(ARRAY_SIZE,1000,VOLTAGE_CODE);

void setup()
{
  attachInterrupt(digitalPinToInterrupt(OPTICAL_PIN), get_delta, FALLING);

  config_rtc();

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

//  Timestamps.add_value(millis());
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
  Serial.println(micros() - last_time);
  last_time = micros();
}

void Task1code( void * parameter) {
  Serial.print("Core: ");Serial.println(xPortGetCoreID());
  for(;;) {
    sendJson();
    UBaseType_t uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL );
    String mensagem = "Core:" + String((int)xPortGetCoreID()) + " --> Stack used: " + String((uint32_t)uxHighWaterMark);
    Serial.println(mensagem);
    vTaskDelay( _5s );
  }
}

void config_rtc(){
  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  
  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
  }

  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}

//void printLocalTime(){
//  struct tm timeinfo;
//  if(!getLocalTime(&timeinfo)){
//    Serial.println("Failed to obtain time");
//    return;
//  }
//  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
//  Serial.print("Day of week: ");
//  Serial.println(&timeinfo, "%A");
//  Serial.print("Month: ");
//  Serial.println(&timeinfo, "%B");
//  Serial.print("Day of Month: ");
//  Serial.println(&timeinfo, "%d");
//  Serial.print("Year: ");
//  Serial.println(&timeinfo, "%Y");
//  Serial.print("Hour: ");
//  Serial.println(&timeinfo, "%H");
//  Serial.print("Hour (12 hour format): ");
//  Serial.println(&timeinfo, "%I");
//  Serial.print("Minute: ");
//  Serial.println(&timeinfo, "%M");
//  Serial.print("Second: ");
//  Serial.println(&timeinfo, "%S");
//
//  Serial.println("Time variables");
//  char timeHour[3];
//  strftime(timeHour,3, "%H", &timeinfo);
//  Serial.println(timeHour);
//  char timeWeekDay[10];
//  strftime(timeWeekDay,10, "%A", &timeinfo);
//  Serial.println(timeWeekDay);
//  Serial.println();
//}

void temperatureSensor() {
  DS18B20.requestTemperatures();       // send the command to get temperatures
  tempC = DS18B20.getTempCByIndex(0);  // read temperature in °C
  Temperatures.add_value(tempC);
}

void setup_imu(){
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
  Accy.add_value(accY);
  Accz.add_value(accZ);
}

void opticalSensor() {
  //int rotacoes = opticalSensorProcess(DELAY_, OPTICAL_PIN);
  int rotations=0;
  unsigned long _sum_dt=0;
  int _count=0;
  long starttime = millis();
  long endtime = starttime;
  while((endtime-starttime)<=OPT_LOOP_TIME_){
    if(dt_vel){
//      rotations = 60000000/dt_vel;
//      sum_rotations += 60000000/dt_vel;
      _sum_dt += dt_vel;
      _count++;
      vTaskDelay( _10ms );
      //Serial.println(rotacoes);
    }
    endtime = millis();
  }
  //sendPacket(String(ROTATIONS_CODE) + " : " + String(rotacoes));
//  Serial.print(_count); Serial.print("$");Serial.println(_sum_dt);
  if(_sum_dt) 
    rotations = 60000000/(_sum_dt/_count);
  _sum_dt = 0;
  _count = 0;
  if(rotations < 10000) 
    Rotations.add_value(rotations);
   else
    Rotations.add_value(-201);
}

void get_delta(){   // interrupt
    dt_vel = micros() - timer_aux;                                       // Calcula delta de tempo entre ultimo pulso e instante atual
    timer_aux = micros();                                                // Atualiza instante do ultimo pulso
}

void voltageSensor() {
  long starttime = millis();
  long endtime = starttime;
  while((endtime-starttime)<=VOLT_LOOP_TIME_) {
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

void sendJson(){
//  DynamicJsonDocument sensors_msg(2048);
//  sensors_msg["type"]='s';
//  JsonArray timestampMcu_data = sensors_msg.createNestedArray(String(TIME_CODE));
//  JsonArray rpm_data = sensors_msg.createNestedArray(String(ROTATIONS_CODE));
//  JsonArray voltage_data = sensors_msg.createNestedArray(String(VOLTAGE_CODE));
//  JsonArray xAcc_data = sensors_msg.createNestedArray(String(ACC_X_CODE));
//  JsonArray yAcc_data = sensors_msg.createNestedArray(String(ACC_Y_CODE));
//  JsonArray zAcc_data = sensors_msg.createNestedArray(String(ACC_Z_CODE));
//  JsonArray temperature_data = sensors_msg.createNestedArray(String(TEMPERATURE_CODE));
//  JsonArray factors = sensors_msg.createNestedArray(String(FACTORS_CODE));
//  factors[TIME_CODE] = Timestamps.get_factor();
//  factors[ROTATIONS_CODE] = Rotations.get_factor();
//  factors[VOLTAGE_CODE] = Voltages.get_factor();
//  factors[ACC_X_CODE] = Accx.get_factor();
//  factors[ACC_Y_CODE] = Accy.get_factor();
//  factors[ACC_Z_CODE] = Accz.get_factor();
//  factors[TEMPERATURE_CODE] = Temperatures.get_factor();
//  for(int i=0;i<ARRAY_SIZE;i++){
//    int res=0;
////    res += addValue(Timestamps.get_value_at_index(i),timestampMcu_data);
//    res += addValue(Rotations.get_value_at_index(i),rpm_data);
//    res += addValue(Voltages.get_value_at_index(i),voltage_data);
//    res += addValue(Accx.get_value_at_index(i),xAcc_data);
//    res += addValue(Accy.get_value_at_index(i),yAcc_data);
//    res += addValue(Accz.get_value_at_index(i),zAcc_data);
//    res += addValue(Temperatures.get_value_at_index(i),temperature_data);
//    if(res==0)
//      break;
//  }
//
//  String json;
//  serializeJson(sensors_msg, json);
//  sendPacket(json);
//  Serial.println(json);
//
////  Timestamps.clear_array();
//  Rotations.clear_array();
//  Voltages.clear_array();
//  Accx.clear_array();
//  Accy.clear_array();
//  Accz.clear_array();
//  Temperatures.clear_array();
//
//  DynamicJsonDocument tstamps_msg(1024);
//  tstamps_msg["type"]='t';
//  JsonArray timestampMcu_data = tstamps_msg.createNestedArray(String(TIME_CODE));
//  for(int i=0;i<ARRAY_SIZE;i++){
//    int res=0;
//    res = addValue(Timestamps.get_value_at_index(i),timestampMcu_data);
//    if(res==0)
//      break;
//  }
//
  publish_all_values();
}

int addValue(float value,JsonArray json_array){
  if(value!=-200 && value!=-201){
    json_array.add(value);
    return 1; 
  }
  return 0;
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
  counter += 6; 
}

void logo()
{
  Heltec.display->clear();
  Heltec.display->drawXbm(0,5,logo_width,logo_height,logo_bits);
  Heltec.display->display();
}
