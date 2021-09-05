#include "opticalSensorReading.h"

unsigned long starttime;
unsigned long endtime;
int rotacoes;

int opticalSensorProcess(long delay, int digital_pin) {
  starttime = millis();
  endtime = starttime;
  rotacoes = 0;
  
  while((endtime-starttime)<=delay) {
    if(digitalRead(digital_pin) == LOW){
        rotacoes++;
        while(digitalRead(digital_pin) == LOW) {
          digitalWrite(LED, HIGH); //turn on the led
        }
    }
    digitalWrite(LED, LOW); //turn off the led
    endtime = millis();
  }

  return rotacoes;

}
