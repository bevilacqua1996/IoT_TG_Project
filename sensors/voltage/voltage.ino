#include <TrueRMSNew.h>
#include <movingAvg.h>

//#include <digitalWriteFast.h> // It uses digitalWriteFast only for the purpose of debugging!
                              // https://code.google.com/archive/p/digitalwritefast/downloads

#define LPERIOD 100    // loop period time in us. In this case 100 us
#define ADC_INPUT 36     // define the used ADC input channel
//#define RMS_WINDOW 40   // rms window of 40 samples, means 2 periods @50Hz
#define RMS_WINDOW 5000   // rms window of 1667 samples, means 10 periods @60Hz

#define PIN_DEBUG 4

unsigned long nextLoop;
int adcVal;
int cnt=0;
float VoltRange = 3.30; // The full scale value is set to 5.00 Volts but can be changed when using an
                        // input scaling circuit in front of the ADC.
unsigned long last_time = 0;

Rms readRms; // create an instance of Rms.
Rms2 readRms2;
movingAvg voltage(500);

void setup() {
  // run once:
  Serial.begin(115200);
  pinMode(PIN_DEBUG, OUTPUT);
  voltage.begin();
  
  // configure for automatic base-line restoration and continuous scan mode:
  readRms.begin(VoltRange, RMS_WINDOW, ADC_12BIT, BLR_ON, CNT_SCAN);
  
  // configure for no baseline restauration and single scan mode:
  //readRms.begin(VoltRange, RMS_WINDOW, ADC_10BIT, BLR_OFF, SGL_SCAN);
  readRms.start(); //start measuring

  //  readRms2.begin(VoltRange, RMS_WINDOW, ADC_12BIT, BLR_OFF, CNT_SCAN);
  //  readRms2.start();
  
  nextLoop = micros() + LPERIOD; // Set the loop timer variable for the next loop interval.
  }

void loop() {
  // run repeatedly:
  adcVal = analogRead(ADC_INPUT); // read the ADC.
  
  while(nextLoop > micros());  // wait until the end of the loop time interval
  nextLoop += LPERIOD;  // set next loop time to current time + LOOP_PERIOD
  
  readRms.update(adcVal); // update
//  readRms2.update(adcVal);

  cnt++;
  if(cnt >= 10000) { // publish every 0.5s
    readRms.publish();
    Serial.println(540.0*readRms.rmsVal,2);
    Serial.println(micros()-last_time); // teste
    last_time = micros(); // teste
    
//    readRms2.publish();
//    Serial.println(79.4*readRms2.rmsVal,2);
    cnt=0;
    //readRms.start();  // Restart the acquisition after publishing if the mode is single scan.
  }
}
