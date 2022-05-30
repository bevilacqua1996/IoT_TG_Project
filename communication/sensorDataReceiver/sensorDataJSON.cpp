#include "sensorDataJSON.h"

int rpm;
long timestamp;
double voltageRMS;
double xAcc;
double yAcc;
double zAcc;
double temperature;

String data_type;

void parsePacket(String packet) {
  String rpm_string;
  String timestamp_string;
  String voltage_string;
  String x_acc_string;
  String y_acc_string;
  String z_acc_string;
  String temperature_string;

  char *cell;
  char buff[packet.length()];
  packet.toCharArray(buff, packet.length());
  cell = strtok(buff, ",");
  while (cell) {
//    Serial.println(String(cell));
    cell = strtok(NULL, ",");
        
    data_type = getValue(cell, 0);
  
    if(data_type.toInt() == ROTATIONS_CODE) {
      rpm_string = getValue(cell, 1);
      rpm = rpm_string.toInt();
    } else if(data_type.toInt() == VOLTAGE_CODE) {
      voltage_string = getValue(cell, 1);
      voltageRMS = atof(voltage_string.c_str());
    } else if(data_type.toInt() == ACC_X_CODE) {
      x_acc_string = getValue(cell, 1);
      xAcc = atof(x_acc_string.c_str());
    } else if(data_type.toInt() == ACC_Y_CODE) {
      y_acc_string = getValue(cell, 1);
      yAcc = atof(y_acc_string.c_str());
    } else if(data_type.toInt() == ACC_Z_CODE) {
      z_acc_string = getValue(cell, 1);
      zAcc = atof(z_acc_string.c_str());
    } else if(data_type.toInt() == TEMPERATURE_CODE) {
      temperature_string = getValue(cell, 1);
      temperature = atof(temperature_string.c_str());
    }
  }
  
//  data_type = getValue(packet, 0);
//  
//  if(data_type.toInt() == ROTATIONS_CODE) {
//    rpm_string = getValue(packet, 1);
//    rpm = rpm_string.toInt();
//  } else if(data_type.toInt() == VOLTAGE_CODE) {
//    voltage_string = getValue(packet, 1);
//    voltageRMS = atof(voltage_string.c_str());
//  } else if(data_type.toInt() == ACC_X_CODE) {
//    x_acc_string = getValue(packet, 1);
//    xAcc = atof(x_acc_string.c_str());
//  } else if(data_type.toInt() == ACC_Y_CODE) {
//    y_acc_string = getValue(packet, 1);
//    yAcc = atof(y_acc_string.c_str());
//  } else if(data_type.toInt() == ACC_Z_CODE) {
//    z_acc_string = getValue(packet, 1);
//    zAcc = atof(z_acc_string.c_str());
//  } else if(data_type.toInt() == TEMPERATURE_CODE) {
//    temperature_string = getValue(packet, 1);
//    temperature = atof(temperature_string.c_str());
//  }

  timestamp = millis();

}

String buildJSON(String packet) {

  parsePacket(packet);

  DynamicJsonDocument doc(1024);

//  if(data_type.toInt() == ROTATIONS_CODE) {
//    doc["rpm"] = rpm;
//  } else if(data_type.toInt() == VOLTAGE_CODE) {
//    doc["voltage"] = voltageRMS;
//  } else if(data_type.toInt() == ACC_X_CODE) {
//    doc["xAcc"] = xAcc;
//  } else if(data_type.toInt() == ACC_Y_CODE) {
//    doc["yAcc"] = yAcc;
//  } else if(data_type.toInt() == ACC_Z_CODE) {
//    doc["zAcc"] = zAcc;
//  } else if(data_type.toInt() == TEMPERATURE_CODE) {
//    doc["temperature"] = temperature;
//  }
  doc["rpm"] = rpm;
  doc["voltage"] = voltageRMS;
  doc["xAcc"] = xAcc;
  doc["yAcc"] = yAcc;
  doc["zAcc"] = zAcc;
  doc["temperature"] = temperature;
  
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

double getXAcc() {
  return xAcc;
}

double getYAcc() {
  return yAcc;
}

double getZAcc() {
  return zAcc;
}

double getTemperatureC() {
  return temperature;
}
