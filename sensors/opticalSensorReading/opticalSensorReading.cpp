#include "opticalSensorReading.h"

unsigned long starttime;
unsigned long endtime;
int rotacoes;
unsigned long timer_aux, dt_vel;

int opticalSensorProcess(long delay, int digital_pin) {
  starttime = millis();
  endtime = starttime;
  rotacoes = 0;
  
  while((endtime-starttime)<=delay) {
    // if(digitalRead(digital_pin) == LOW){
        // rotacoes++;
		// //digitalWrite(LED, HIGH); //turn on the led
        // while(digitalRead(digital_pin) == LOW) {
          // //digitalWrite(LED, HIGH); //turn on the led
        // }
    // }
    //digitalWrite(LED, LOW); //turn off the led
	
	if(dt_vel)
		rotacoes = 1000000/dt_vel*60;
	
    endtime = millis();
  }
  delay(10);

  return rotacoes*1000/delay*60;

}

void get_delta(){   // interrupt
    dt_vel = micros() - timer_aux;                                       // Calcula delta de tempo entre ultimo pulso e instante atual
    timer_aux = micros();                                                // Atualiza instante do ultimo pulso
}
