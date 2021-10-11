#include <EmonLib.h>
#define pino_corrente A1

const int N_voltas = 2000;
const int R_burden = 33;
const int num_samples = 500;
int readVal[num_samples];
int n=0;

unsigned long tempo;

EnergyMonitor emon1;

void setup() {
  emon1.current(pino_corrente,N_voltas/R_burden);
  Serial.begin(115200);
  tempo = micros();
}

// the loop routine runs over and over again forever:
void loop() {
  int sensorValue = analogRead(pino_corrente);
  delayMicroseconds(250);
  readVal[n++] = sensorValue;
//  double Irms = emon1.calcIrms(1480);
  //Serial.println(sensorValue); 
//  Serial.print(",");
//  Serial.println(Irms);
  if(n==num_samples){
    imprime_dados();
  }
//  delay(30);        // delay in between reads for stability
//delayMicroseconds(100);
}

void imprime_dados(){
//  Serial.println(micros()-tempo);
  for(int i=0;i<num_samples;i++)
    Serial.println(readVal[i]);
  for(int i=0;i<num_samples;i++)
    readVal[i] = 0;
  n=0;
}
