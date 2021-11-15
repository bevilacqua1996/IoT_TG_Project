#include "sensorDataJSON.h"

int rpm;
long timestamp;
double voltageRMS;

String data_type;

void parsePacket(String packet) {
  String rpm_string;
  String timestamp_string;
  String voltage_string;
  
  data_type = getValue(packet, 0);
  
  if(data_type.toInt() == ROTATIONS_CODE) {
    rpm_string = getValue(packet, 1);
    rpm = rpm_string.toInt();
  } else if(data_type.toInt() == VOLTAGE_CODE) {
    voltage_string = getValue(packet, 1);
    voltageRMS = atof(voltage_string.c_str());
  }

  timestamp = millis();

}

String buildJSON(String packet) {

  parsePacket(packet);

  DynamicJsonDocument doc(1024);

  if(data_type.toInt() == ROTATIONS_CODE) {
    doc["rpm"] = rpm;
  } else if(data_type.toInt() == VOLTAGE_CODE) {
    doc["voltage"] = voltageRMS;
  }
  doc["timestampMicrocontroler"] = timestamp;

  String json;
  serializeJson(doc, json);

  return json;
}

long getTimestamp() {
  return timestamp;
}

int getRpm() {
  return rpm;
}

double getVoltage() {
  return voltageRMS;
}
