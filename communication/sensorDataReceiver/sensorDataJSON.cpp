#include "sensorDataJSON.h"
#define ARRAY_SIZE 10

int rpm;
long timestamp;
double voltageRMS;
double xAcc;
double yAcc;
double zAcc;
double temperature;

//int storage_int_array[ARRAY_SIZE] = {-200};
//long storage_long_array[ARRAY_SIZE] = {-200};
//double storage_double_array[ARRAY_SIZE] = {0.0};
//Vector<int> rpm_array(storage_int_array);
//Vector<long> timestamp_array(storage_long_array);
//Vector<double> voltageRMS_array(storage_double_array);
//Vector<double> xAcc_array(storage_double_array);
//Vector<double> yAcc_array(storage_double_array);
//Vector<double> zAcc_array(storage_double_array);
//Vector<double> temp_array(storage_double_array);

class JsonValues{
  public:
    JsonValues(unsigned short array_size, unsigned short factor){
      this->array_size = array_size;
      this->values = new int[array_size];
      std::fill_n(this->values,array_size,-200);
      this->factor = factor;
    }
    
    void add_value(float value) volatile{
      if(this->index < this->array_size){
        this->values[index] = value*factor;
        this->index++;
      }
    }
    
    float get_value_at_index(short index){
      if (this->values[index] != -200)
        return this->values[index]/float(factor);
      return this->values[index];
    }

    void clear_array(){
      std::fill_n(values,array_size,-200);
      index=0;
    }
    
  private:
    unsigned short index;
    int *values;
    unsigned short array_size;
    unsigned short factor;
};

JsonValues Rotations = JsonValues(ARRAY_SIZE,1000);
JsonValues Temperatures = JsonValues(ARRAY_SIZE,100);
JsonValues Accx = JsonValues(ARRAY_SIZE,1000);
JsonValues Accy = JsonValues(ARRAY_SIZE,1000);
JsonValues Accz = JsonValues(ARRAY_SIZE,1000);
JsonValues Voltages = JsonValues(ARRAY_SIZE,1000);
JsonValues Timestamps = JsonValues(ARRAY_SIZE,1000);

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
  Serial.println(packet);
  while (cell) {
//    Serial.println(String(cell));
    cell = strtok(NULL, ",");
        
    data_type = getValue(cell, 0);
  
    if(data_type.toInt() == ROTATIONS_CODE) {
      rpm_string = getValue(cell, 1);
//      rpm = rpm_string.toInt();
//      rpm_array.push_back(rpm_string.toInt());
      Rotations.add_value(rpm_string.toInt());
    } else if(data_type.toInt() == VOLTAGE_CODE) {
      voltage_string = getValue(cell, 1);
//      voltageRMS = atof(voltage_string.c_str());
//      voltageRMS_array.push_back(atof(voltage_string.c_str()));
      Voltages.add_value(atof(voltage_string.c_str()));
    } else if(data_type.toInt() == ACC_X_CODE) {
      x_acc_string = getValue(cell, 1);
//      xAcc = atof(x_acc_string.c_str());
//      xAcc_array.push_back(atof(x_acc_string.c_str()));
      Accx.add_value(atof(x_acc_string.c_str()));
    } else if(data_type.toInt() == ACC_Y_CODE) {
      y_acc_string = getValue(cell, 1);
//      yAcc = atof(y_acc_string.c_str());
//      yAcc_array.push_back(atof(y_acc_string.c_str()));
      Accy.add_value(atof(y_acc_string.c_str()));
    } else if(data_type.toInt() == ACC_Z_CODE) {
      z_acc_string = getValue(cell, 1);
//      zAcc = atof(z_acc_string.c_str());
//      zAcc_array.push_back(atof(z_acc_string.c_str()));
      Accz.add_value(atof(z_acc_string.c_str()));
    } else if(data_type.toInt() == TEMPERATURE_CODE) {
      temperature_string = getValue(cell, 1);
//      temperature = atof(temperature_string.c_str());
//      temp_array.push_back(atof(temperature_string.c_str()));
      Temperatures.add_value(atof(temperature_string.c_str()));
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

  JsonArray rpm_data = doc.createNestedArray("rpm_data");
  JsonArray voltage_data = doc.createNestedArray("voltage_data");
  JsonArray xAcc_data = doc.createNestedArray("xAcc_data");
  JsonArray yAcc_data = doc.createNestedArray("yAcc_data");
  JsonArray zAcc_data = doc.createNestedArray("zAcc_data");
  JsonArray temperature_data = doc.createNestedArray("temperature_data");
  JsonArray timestampMcu_data = doc.createNestedArray("timestampMcu_data");
//  for(int i=0;i<rpm_array.size();i++){
//    rpm_data.add(rpm_array[i]);
//    voltage_data.add(voltageRMS_array[i]);
//    xAcc_data.add(xAcc_array[i]);
//    yAcc_data.add(yAcc_array[i]);
//    zAcc_data.add(zAcc_array[i]);
//    temperature_data.add(temp_array[i]);
//    timestampMcu_data.add(timestamp_array[i]);
//  }
  for(int i=0;i<ARRAY_SIZE;i++){
    rpm_data.add(Rotations.get_value_at_index(i));
    voltage_data.add(Voltages.get_value_at_index(i));
    xAcc_data.add(Accx.get_value_at_index(i));
    yAcc_data.add(Accy.get_value_at_index(i));
    zAcc_data.add(Accz.get_value_at_index(i));
    temperature_data.add(Temperatures.get_value_at_index(i));
    timestampMcu_data.add(Timestamps.get_value_at_index(i));
  }
  Rotations.clear_array();
  Voltages.clear_array();
  Accx.clear_array();
  Accy.clear_array();
  Accz.clear_array();
  Temperatures.clear_array();
  Timestamps.clear_array();
  
//  doc["rpm"] = rpm;
//  doc["voltage"] = voltageRMS;
//  doc["xAcc"] = xAcc;
//  doc["yAcc"] = yAcc;
//  doc["zAcc"] = zAcc;
//  doc["temperature"] = temperature;
//  doc["timestampMicrocontroler"] = timestamp;

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
