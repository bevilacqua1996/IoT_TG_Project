#include "sensorDataJSON.h"

int rpm;
long timestamp;

void parsePacket(String packet) {
  String rpm_string = getValue(packet, 0);
  String timestamp_string = getValue(packet, 1);

  int n = timestamp_string.length();
 
  char timestamp_char[n + 1];

  strcpy(timestamp_char, timestamp_string.c_str());

  rpm = rpm_string.toInt();
  timestamp = atol(timestamp_char);

}

String buildJSON(String packet) {

  parsePacket(packet);

  DynamicJsonDocument doc(1024);
  doc["rpm"] = rpm;
  doc["timestampMicrocontroler"] = timestamp;

  String json;
  serializeJson(doc, json);

  return json;
}
