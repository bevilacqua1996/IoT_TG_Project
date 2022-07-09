/*
 * Camada de comunicação LoRa e API
 * 
 * Função: Sender, envio de dados para LoRa receiver
*/

#include <heltec.h>
#include "images.h"
#include <TrueRMSNew.h>
#include <acc.h>
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <WiFi.h>
#include <WiFiUDP.h>

#define TIME_CODE 0
#define ROTATIONS_CODE 1
#define VOLTAGE_CODE 2
#define ACC_A1_CODE 3
#define ACC_A2_CODE 4
#define ACC_A3_CODE 5
#define TEMPERATURE_CODE 6

#define BAND    915E6  //you can set band here directly,e.g. 868E6,915E6

#define LPERIOD 100    // loop period time in us. In this case 100 us
#define VOLTAGE_PIN 36     // define the used ADC input channel
#define RMS_WINDOW 5000   // rms window of 1667 samples, means 10 periods @60Hz

#define Register_2D 0x2D

#define TEMPERATURE_PIN 32 // ESP32 pin GIOP33 connected to DS18B20 sensor's DQ pin

#define max_rotation 10000
#define min_rotation 10

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
const long DELAY_ = 2000;
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
const TickType_t _200ms = 200 / portTICK_PERIOD_MS;
const TickType_t _5s = 5000 / portTICK_PERIOD_MS; 
const TickType_t _10s = 10000 / portTICK_PERIOD_MS; 
TaskHandle_t Task1;
static String tmp="";

class SensorValues{
  public:
    SensorValues(unsigned short array_size, unsigned short factor, short code){
      this->array_size = array_size;
      this->values = new int[array_size];
//      this->timestamps = new uint32_t[array_size];
      std::fill_n(this->values,array_size,-200);
//      std::fill_n(this->timestamps,array_size,-200);
      this->factor = factor;
      this->code = code;
    }
    
    void add_value(float value) volatile{
      if(!is_reading){
        if(this->index < this->array_size){
          is_writing = 1;
          this->values[index] = value*factor;
//          this->timestamps[index] = time(NULL);
          this->index++;
          is_writing = 0;
        }
      }
    }

    void add_int_value(uint32_t value) volatile{
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

//    uint32_t get_timestamp_at_index(short index) volatile{
//      return this->timestamps[index];
//    }

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
      msg["id"] = pkg_id;
      msg["code"] = this->code;
      msg["factor"] = this->factor;
      JsonArray var_data = msg.createNestedArray("data");
//      JsonArray tStamps = msg.createNestedArray("ts");
      for(int i=0;i<this->array_size;i++){
        if(this->values[i]==-200)
          break;
        var_data.add(this->values[i]);
//        tStamps.add(this->timestamps[i]);
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
//    volatile uint32_t* timestamps;
    volatile unsigned short array_size;
    volatile unsigned short factor;
    char code;
};

volatile SensorValues Timestamps = SensorValues(ARRAY_SIZE,1,TIME_CODE);
volatile SensorValues Rotations = SensorValues(ARRAY_SIZE,1000,ROTATIONS_CODE);
volatile SensorValues Temperatures = SensorValues(ARRAY_SIZE,100,TEMPERATURE_CODE);
volatile SensorValues AccA1 = SensorValues(ARRAY_SIZE,1000,ACC_A1_CODE);
volatile SensorValues AccA2 = SensorValues(ARRAY_SIZE,1000,ACC_A2_CODE);
volatile SensorValues AccA3 = SensorValues(ARRAY_SIZE,1000,ACC_A3_CODE);
volatile SensorValues Voltages = SensorValues(ARRAY_SIZE,1000,VOLTAGE_CODE);

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

  DS18B20.begin();    // initialize the DS18B20 sensor
  
   //WIFI Kit series V1 not support Vext control
  Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.Heltec.Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);
//  Serial.begin(115200);
 
  Heltec.display->init();
  Heltec.display->flipScreenVertically();  
  Heltec.display->setFont(ArialMT_Plain_10);
  logo();
  delay(1500);
  Heltec.display->clear();
  
  Heltec.display->drawString(0, 0, "Heltec.LoRa Initial success!");
  Heltec.display->display();
  delay(1000);

  config_rtc();
  //calibrateAcc();

  //pinMode(OPTICAL_PIN, INPUT);
  LoRa.setSpreadingFactor(7);
  LoRa.setSignalBandwidth(250E3);
}

void loop()
{
  Heltec.display->clear();
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->setFont(ArialMT_Plain_10);
  
  Heltec.display->drawString(0, 0, "Sending packet: ");
  Heltec.display->drawString(90, 0, String(counter));
  Heltec.display->display();

  Timestamps.add_int_value(time(NULL));
  voltageSensor();
  opticalSensor();
  accSensor();
  temperatureSensor();
  vTaskDelay( _100ms );
//  vTaskDelay( _1s );
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
  fft_scan();
  float fp = get_pass_pole_freq(); 
  float A1 = get_amplitude_at_freq(60);
  //float A1 = get_amplitude_at_2fl();
  float A2 = get_amplitude_at_lateral_band(120,120/3)[0];
  float A3 = get_amplitude_at_lateral_band(120,fp)[0];
  Serial.print("Amplitude: "); Serial.println(A1);
  
  AccA1.add_value(A1);
  AccA2.add_value(A2);
  AccA3.add_value(A3);
}

void opticalSensor() {
  //int rotacoes = opticalSensorProcess(DELAY_, OPTICAL_PIN)
  unsigned long rotations=0;
  unsigned long _sum_dt=0;
  int _count=0;
  long starttime = millis();
  long endtime = starttime;
  while((endtime-starttime)<=OPT_LOOP_TIME_){
    if(dt_vel>0 && micros()-timer_aux < 1000000/min_rotation){
      _sum_dt += dt_vel;
      _count++;
      vTaskDelay( _10ms );
      //Serial.println(rotacoes);
    }
    endtime = millis();
  }

  if(_sum_dt) 
    rotations = 60000000/(_sum_dt/_count);
  _sum_dt = 0;
  _count = 0;
  if(rotations < max_rotation) 
    Rotations.add_value(rotations);
   else
    Rotations.add_value(-200);
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
  Voltages.add_value(540.0*readRms.rmsVal);
}

void sendJson(){
  publish_all_values();
  pkg_id++;
}

//int addValue(float value,JsonArray json_array){
//  if(value!=-200 && value!=-201){
//    json_array.add(value);
//    return 1; 
//  }
//  return 0;
//}

void sendPacket(String message) {
  // send packet
  LoRa.beginPacket();
  
  LoRa.setTxPower(14,RF_PACONFIG_PASELECT_PABOOST);
  LoRa.print(message);
  LoRa.endPacket(true);
  counter++;
}

void publish_all_values(){
  Timestamps.publish_values();
  vTaskDelay(_200ms);
  Rotations.publish_values();
  vTaskDelay(_200ms);
  Voltages.publish_values();
  vTaskDelay(_200ms);
  AccA1.publish_values();
  vTaskDelay(_200ms);
  AccA2.publish_values();
  vTaskDelay(_200ms);
  AccA3.publish_values();
  vTaskDelay(_200ms);
  Temperatures.publish_values();
  vTaskDelay(_200ms);
  counter += 7; 
}

void logo()
{
  Heltec.display->clear();
  Heltec.display->drawXbm(0,5,logo_width,logo_height,logo_bits);
  Heltec.display->display();
}
