/*
 * Camada de comunicação LoRa e API
 * 
 * Função: Receiver e POST de dados dos sensores na API
*/
#include "heltec.h" 
#include "images.h"
#include "sensorDataJSON.h"
#include "restMapping.h"

#include <WiFi.h>

#define BAND    915E6  //you can set band here directly,e.g. 868E6,915E6
String rssi = "RSSI --";
String packSize = "--";
String packet ;

const char* ssid = "PUT YOUR SSID HERE";
const char* password = "PUT YOUR NETWORK PASSWORD HERE";

void logo(){
  Heltec.display->clear();
  Heltec.display->drawXbm(0,5,logo_width,logo_height,logo_bits);
  Heltec.display->display();
}

void buildAndSendLoRaData(){

  String json = buildJSON(packet);
  
  int httpResponseCode;
  
  if(WiFi.status()== WL_CONNECTED){
    httpResponseCode = postData(json);
  } else {
    httpResponseCode = 500;
  }
  
  Heltec.display->clear();
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->drawString(0 , 15 , "Received "+ packSize + " bytes");
  Heltec.display->drawString(0 , 40 , "Http Status Code: "+ String(httpResponseCode));
  Heltec.display->drawStringMaxWidth(0 , 26 , 128, packet);
  Heltec.display->drawString(0, 0, rssi);  
  Heltec.display->display();
}

void cbk(int packetSize) {
  packet ="";
  packSize = String(packetSize,DEC);
  for (int i = 0; i < packetSize; i++) { packet += (char) LoRa.read(); }
  rssi = "RSSI " + String(LoRa.packetRssi(), DEC) ;
  buildAndSendLoRaData();
}

void wifiConfigure() {
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  Heltec.display->clear();

  Heltec.display->drawString(0, 10, "Wifi Connected!");
  Heltec.display->display();

  delay(1500);
}

void setup() {

  Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.Heltec.Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);

  Heltec.display->init();
  Heltec.display->flipScreenVertically();  
  Heltec.display->setFont(ArialMT_Plain_10);
  logo();
  delay(1500);

  wifiConfigure();

  Heltec.display->clear();
  
  Heltec.display->drawString(0, 0, "Heltec.LoRa Initial success!");
  Heltec.display->drawString(0, 10, "Wait for incoming data...");
  Heltec.display->display();
  delay(1000);
  LoRa.receive();
  
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) { cbk(packetSize);  }
  delay(10);
}