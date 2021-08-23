#include <OneWire.h>
#include <DallasTemperature.h>

#define DS18B20 A0

OneWire onewire(DS18B20);
DallasTemperature sensores(&onewire);

void setup() {
  sensores.begin();
  //pinMode(A0,OUTPUT);
  Serial.begin(9600);
}

void loop() {
  sensores.requestTemperatures();
  Serial.print("Temperatura: "); 
  Serial.println(sensores.getTempCByIndex(0));
  delay(250);     
}
