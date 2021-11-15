/*
 * Camada de comunicação LoRa e API
 * 
 * Função: Sender, envio de dados para LoRa receiver
*/

#include "heltec.h"
#include "images.h"
#include <protothreads.h>
#include <opticalSensorReading.h>
#include <TrueRMSNew.h>

#define ROTATIONS_CODE 1
#define VOLTAGE_CODE 2

#define BAND    915E6  //you can set band here directly,e.g. 868E6,915E6

#define LPERIOD 100    // loop period time in us. In this case 100 us
#define ADC_INPUT 0     // define the used ADC input channel
#define RMS_WINDOW 5000   // rms window of 1667 samples, means 10 periods @60Hz

unsigned int counter = 0;
String rssi = "RSSI --";
String packSize = "--";
String packet;

const long DELAY_ = 60000;
const int digital_pin = 23; // possible digital Input for LoRa32

Rms readRms; // create an instance of Rms.

unsigned long nextLoop;
int adcVal;
int cnt=0;
float VoltRange = 3.30; // The full scale value is set to 5.00 Volts but can be changed when using an
                        // input scaling circuit in front of the ADC.
unsigned long last_time = 0;

static struct pt ptOpticalSensor;
static struct pt ptVoltageSensor;

void setup()
{
  // configure for automatic base-line restoration and continuous scan mode:
  readRms.begin(VoltRange, RMS_WINDOW, ADC_12BIT, BLR_ON, CNT_SCAN);
  
  // configure for no baseline restauration and single scan mode:
  readRms.start(); //start measuring
  
  nextLoop = micros() + LPERIOD; // Set the loop timer variable for the next loop interval.
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

  pinMode(digital_pin, INPUT);

  PT_INIT(&ptOpticalSensor);
  PT_INIT(&ptVoltageSensor);
  
}

void loop()
{
  Heltec.display->clear();
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->setFont(ArialMT_Plain_10);
  
  Heltec.display->drawString(0, 0, "Sending packet: ");
  Heltec.display->drawString(90, 0, String(counter));
  Heltec.display->display();

  protothreadVoltageSensor(&ptVoltageSensor);
  protothreadOpticalSensor(&ptOpticalSensor);

}

static int protothreadOpticalSensor(struct pt *pt) {
  PT_BEGIN(pt);
  opticalSensor();
  PT_END(pt);
}

void opticalSensor() {
  int rotacoes = opticalSensorProcess(DELAY_, digital_pin);
  sendPacket(String(ROTATIONS_CODE) + " : " + String(rotacoes));
}

static int protothreadVoltageSensor(struct pt *pt) {
  PT_BEGIN(pt);
  voltageSensor();
  PT_END(pt); 
}

void voltageSensor() {
  long starttime = millis();
  long endtime = starttime;
  while((endtime-starttime)<=DELAY_) {
    adcVal = analogRead(ADC_INPUT); // read the ADC.
    readRms.update(adcVal); // update
    endtime = millis();
    readRms.update(adcVal); // update
  }
  readRms.publish();
  sendPacket(String(VOLTAGE_CODE) + " : " + String(540.0*readRms.rmsVal,2));
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
