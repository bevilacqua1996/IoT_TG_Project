#define FFT_SIZE 512  // 32/64/128/256/512
#include <FFT_C.h>

//static int *raw1 = new int[FFT_SIZE];
static int *raw2 = new int[FFT_SIZE];

static int spectr[FFT_SIZE];
//long spectr[FFT_SIZE];

float freq=60;
int sample_freq=1000;

void setup() {
  Serial.begin(38400);
  //square(raw1);
  sine(raw2, freq, sample_freq);
}

void loop() {
//  for (int i = 0; i < FFT_SIZE; i++) 
//    raw[i] = analogRead(A0);

////  FFT(raw, spectr);
////  FFT(raw0, spectr);
////  FFT(raw1, spectr);
  FFT(raw2, spectr);
  int low_raw = 0;
  int high_raw = 0;
  for(int j=0;j<1;j++){
    for (int i = 0; i < FFT_SIZE / 2; i++) {
      spectr[i] = (spectr[i] * (i + 2)) >> 1;
      if (i < 2) low_raw += spectr[i];
      else high_raw += spectr[i];
    }
  }
  
  print_fft_spectre();
//  for (int i = 0; i < FFT_SIZE; i++){
//    Serial.println(spectr[i]);
//  }
  while(1);
  //delay(1000);
}

void square(int *raw){
  for(int i=FFT_SIZE/2;i<FFT_SIZE;i++){
    raw[i] = 24;
  }
}

void sine(int *raw, float freq, float fsample){
//  float freq = 78.125;
//  float fsample = 1000;
  for(int i=0;i<FFT_SIZE;i++){
    raw[i] = 24*sin(2*3.1415*freq/fsample*i);
  }
}

void print_fft_spectre(){
  for (int i = 0; i < FFT_SIZE; i++){
    //Serial.print(sample_freq*i/float(FFT_SIZE)); Serial.print("\t");
    Serial.println(spectr[i]);
  }
}
