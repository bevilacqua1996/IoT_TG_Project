/*
 * Este é um exemplo de comunicação LoRa utilizando a 
 * leitura de dados do sensor óptico TCRT5000.
*/

#include "heltec.h"
#include "images.h"

#define BAND    915E6  //you can set band here directly,e.g. 868E6,915E6

unsigned int counter = 0;
String rssi = "RSSI --";
String packSize = "--";
String packet ;
String messageSent = "";
String message = "";

boolean canSend = false;

const String DETECTADO_ = "PRESENCA DETECTADA!";
const String NAO_DETECTADO_ = "NADA DETECTADO...";

const int digital_pin = 23; // possible digital Input for LoRa32
const int internal_LED = 25; // internal LED for LoRa32

void setup()
{
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
  pinMode(internal_LED, OUTPUT);
}

void loop()
{
  Heltec.display->clear();
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->setFont(ArialMT_Plain_10);
  
  Heltec.display->drawString(0, 0, "Sending packet: ");
  Heltec.display->drawString(90, 0, String(counter));
  Heltec.display->display();

  canSend = opticalSensorChanged();

  if(canSend) {
    sendPacket(message);
  }

}

boolean opticalSensorChanged() {
  if(digitalRead(digital_pin) == HIGH){
    digitalWrite(internal_LED, LOW); //turn off the led
    message = NAO_DETECTADO_;
  } else {
    digitalWrite(internal_LED, HIGH); //turn on the led
    message = DETECTADO_;
  }

  if(!message.equals(messageSent)) {
    messageSent = message;
    return true;
  }

  return false;

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
