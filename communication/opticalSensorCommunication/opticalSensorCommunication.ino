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

unsigned long starttime;
unsigned long endtime;
int rotacoes;
const long DELAY_ = 10000;

const int digital_pin = 23; // possible digital Input for LoRa32

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
}

void loop()
{
  Heltec.display->clear();
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->setFont(ArialMT_Plain_10);
  
  Heltec.display->drawString(0, 0, "Sending packet: ");
  Heltec.display->drawString(90, 0, String(counter));
  Heltec.display->display();

  starttime = millis();
  endtime = starttime;
  rotacoes = 0;
  
  while((endtime-starttime)<=DELAY_) {
    opticalSensorReading();
    endtime = millis();
  }

  sendPacket("Detecções = " + String(rotacoes));

}

void opticalSensorReading() {
  if(digitalRead(digital_pin) == LOW){
    rotacoes++;
    while(digitalRead(digital_pin) == LOW) {
      digitalWrite(LED, HIGH); //turn on the led
    }
  }
  digitalWrite(LED, LOW); //turn off the led
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
