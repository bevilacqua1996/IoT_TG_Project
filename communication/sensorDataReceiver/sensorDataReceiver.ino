/*
 * Camada de comunicação LoRa e API
 * 
 * Função: Receiver e POST de dados dos sensores na API
*/
#include "heltec.h" 
#include "images.h"
#include "sensorDataJSON.h"
#include "restMapping.h"
#include "support.h"

#define ROTATIONS_CODE 1
#define VOLTAGE_CODE 2
#define ACC_X_CODE 3
#define ACC_Y_CODE 4
#define ACC_Z_CODE 5
#define TEMPERATURE_CODE 6

//#include <WiFi.h>

#define BAND    915E6  //you can set band here directly,e.g. 868E6,915E6
String rssi = "RSSI --";
String packSize = "--";
String packet ;

extern const char* ssid;
extern const char* password;

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

  String data_type = getValue(packet, 0);
  String type_str;
  String data_str = getValue(packet, 1);
  if(data_type.toInt() == ROTATIONS_CODE) {
    type_str = "Rotations";
  } else if(data_type.toInt() == VOLTAGE_CODE) {
    type_str = "Voltage";
  } else if(data_type.toInt() == ACC_X_CODE) {
    type_str = "Xacc";
  } else if(data_type.toInt() == ACC_Y_CODE) {
    type_str = "Yacc";
  } else if(data_type.toInt() == ACC_Z_CODE) {
    type_str = "Zacc";
  } else if(data_type.toInt() == TEMPERATURE_CODE) {
    type_str = "Temperature Cº";
  }
  
  Heltec.display->clear();
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->drawString(0 , 15 , "Received "+ packSize + " bytes");
  Heltec.display->drawString(0 , 40 , "Http Status Code: "+ String(httpResponseCode));
  Heltec.display->drawStringMaxWidth(0 , 26 , 128, type_str + " : " + data_str);
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
    Serial.println("Trying to conect...");
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
//  Serial.println("A");
  if (packetSize) { cbk(packetSize);  }
  delay(10);
//  delay(1000);
}
