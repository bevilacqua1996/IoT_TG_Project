#include <EmonLib.h>
#include <TrueRMSNew.h>
#define pino_corrente 33

const int N_voltas = 2000;
const int R_burden = 120;
const int num_samples = 500;
int readVal[num_samples];
int n=0;

unsigned long tempo;

//EnergyMonitor emon1;
const float VoltRange = 3.3;
const int RMS_WINDOW = 5000;
const int LPERIOD = 100;
unsigned long nextLoop;
int cnt = 0;

Rms readRms;

void setup() {
  //emon1.current(pino_corrente,N_voltas/R_burden);
  Serial.begin(115200);
  tempo = micros();
  start_Irms();
}

// the loop routine runs over and over again forever:
void loop() {
//  int sensorValue = analogRead(pino_corrente);
//  delayMicroseconds(250);
//  readVal[n++] = sensorValue;
////  double Irms = emon1.calcIrms(1480);
//  //Serial.println(sensorValue); 
////  Serial.print(",");
////  Serial.println(Irms);
//  if(n==num_samples){
//    imprime_dados();
//  }
////  delay(30);        // delay in between reads for stability
////delayMicroseconds(100);

  get_waveform();
//  get_Irms();
}

void get_waveform(){
  int sensorValue = analogRead(pino_corrente);
  delayMicroseconds(250);
  readVal[n++] = sensorValue;
  if(n==num_samples){
    imprime_dados();
  }
}

void start_Irms(){
  readRms.begin(VoltRange, RMS_WINDOW, ADC_12BIT, BLR_ON, CNT_SCAN);
  readRms.start(); //start measuring
  nextLoop = micros() + LPERIOD;
}

void get_Irms(){
  int adcVal = analogRead(pino_corrente);
  while(nextLoop > micros());  // wait until the end of the loop time interval
  nextLoop += LPERIOD;  // set next loop time to current time + LOOP_PERIOD
  
  readRms.update(adcVal);
  cnt++;

  if(cnt >= 10000) { // publish every 0.5s
    readRms.publish();
    Serial.println(N_voltas/R_burden * readRms.rmsVal,4);
//    Serial.println(micros()-last_time); // teste
//    last_time = micros(); // teste
    cnt=0;
  }
}

void imprime_dados(){
//  Serial.println(micros()-tempo);
  for(int i=0;i<num_samples;i++)
    Serial.println(readVal[i]);
  for(int i=0;i<num_samples;i++)
    readVal[i] = 0;
  n=0;
}
