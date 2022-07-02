
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
#include <TimeLib.h>

#define TIME_CODE 0
#define ROTATIONS_CODE 1
#define VOLTAGE_CODE 2
#define ACC_X_CODE 3
#define ACC_Y_CODE 4
#define ACC_Z_CODE 5
#define TEMPERATURE_CODE 6

#define NUM_OF_PACKETS 7
#define len(arr) sizeof (arr)/sizeof (arr[0])

//#include <WiFi.h>

#define BAND    915E6  //you can set band here directly,e.g. 868E6,915E6
String rssi = "RSSI --";
String packSize = "--";
String packet ;
String packets[NUM_OF_PACKETS];
int packet_size = 0;
int packetsReceived = 0;
int pkg_id=-1;

unsigned long tempo;

extern const char* ssid;
extern const char* password;

//List<String> packs;
//List<String> packsSent;
volatile bool canSendPacket = false;

void logo(){
  Heltec.display->clear();
  Heltec.display->drawXbm(0,5,logo_width,logo_height,logo_bits);
  Heltec.display->display();
}

void buildAndSendLoRaData(){
  //String json = buildJSON(packet);
  //int httpResponseCode;
  //if(packetsReceived >= NUM_OF_PACKETS){
  //int packsSize = packs.getSize();
  for(int i=0;i<NUM_OF_PACKETS;i++){
    int httpResponseCode;
    String m = "packet [" + String(i) + "] = " + packets[i];
    Serial.println(m);
    tempo = millis();
    if(WiFi.status()== WL_CONNECTED)
      httpResponseCode = sendJson(packets[i]);
    else
      httpResponseCode = 500;
    displayData(httpResponseCode, packets[i]);
    Serial.print("Loop time: ");  Serial.println(millis()-tempo);  /* Serial.println(tempo); */
  }

  packetsReceived = 0;
  canSendPacket = false;
}

void displayData(int httpResponseCode, String json){  
    DynamicJsonDocument doc(1024);
    //deserializeJson(doc, pckt);
    deserializeJson(doc, json);

    int id = doc["id"];
    int code = doc["code"];
    int value = doc["data"][0];
    int factor = doc["factor"];

    //Serial.print("Value: ");Serial.println(value);
    String data_str = String(value / float(factor));
    String type_str;
    if(code == ROTATIONS_CODE) {
      type_str = "Rotations";
    } else if(code == VOLTAGE_CODE) {
      type_str = "Voltage";
    } else if(code == ACC_X_CODE) {
      type_str = "Xacc";
    } else if(code == ACC_Y_CODE) {
      type_str = "Yacc";
    } else if(code == ACC_Z_CODE) {
      type_str = "Zacc";
    } else if(code == TEMPERATURE_CODE) {
      type_str = "Temp. ºC";
    } else if(code == TIME_CODE){
      type_str = "Time";
      String hms = String((24+hour(value)-3)%24) + ":" + String(minute(value)) + ":" + String(second(value));
      data_str = hms;
    }
    
    Heltec.display->clear();
    Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
    Heltec.display->setFont(ArialMT_Plain_10);
    Heltec.display->drawString(0 , 15 , "Received "+ packSize + " bytes");
    Heltec.display->drawString(0 , 40 , "Http Status Code: "+ String(httpResponseCode));
    Heltec.display->drawStringMaxWidth(0 , 26 , 128, type_str + " : " + data_str);
    Heltec.display->drawString(0, 0, rssi);  
    Heltec.display->display();
    if(code == TIME_CODE)
      delay(500);
    else
      delay(150);
  //}
}

int sendJson(String json){
  int httpResponseCode;
//  if(WiFi.status()== WL_CONNECTED){
//      httpResponseCode = postData(json);
//  }
//  else {
//      httpResponseCode = 500;
//  }
  httpResponseCode = postData(json);
  return httpResponseCode;
}

void onReceive(int packetSize){
  if (packetSize) {
    packet_size = packetSize;
    //tempo = millis();
    //Serial.print("Packet size: ");Serial.println(packetSize); 
    cbk(packetSize);
    //Serial.print("Loop time: ");  Serial.println(millis()-tempo);  /* Serial.println(tempo); */
  }
}

void cbk(int packetSize) {
  packet ="";
  packSize = String(packetSize,DEC);
  for (int i = 0; i < packetSize; i++) { packet += (char) LoRa.read(); }
  rssi = "RSSI " + String(LoRa.packetRssi(), DEC) ;
  //Serial.println(packet);

  DynamicJsonDocument doc(2048);
  deserializeJson(doc, packet);

  int id = doc["id"];
  int code = doc["code"];
 
  packets[code] = packet;
  packetsReceived++;
  if(packetsReceived>=NUM_OF_PACKETS){
    canSendPacket = true;
    packetsReceived = 0;
  }
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

  LoRa.setSpreadingFactor(7);
  LoRa.setSignalBandwidth(250E3);
  
  LoRa.receive();
  LoRa.onReceive(onReceive);
}

void loop() {
//  int packetSize = LoRa.parsePacket();
//  if (packetSize) {
//    tempo = millis();
//    Serial.print("Packet size: ");Serial.println(packetSize); 
//    cbk(packetSize); 
//    Serial.print("Loop time: ");  Serial.println(millis()-tempo);  /* Serial.println(tempo); */
//    tempo = millis();
//  }
  delay(10);

  if(canSendPacket){
    buildAndSendLoRaData();
  }
}
