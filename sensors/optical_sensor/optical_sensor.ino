// for ESP32 microcontroller
const int digital_pin = 23; // possible digital Input for LoRa32
const int internal_LED = 25; // internal LED for LoRa32

void setup(){
  pinMode(digital_pin, INPUT);
  pinMode(internal_LED, OUTPUT);
}

void loop(){
  if(digitalRead(digital_pin) == HIGH){
    digitalWrite(internal_LED, LOW); //turn off the led
  }
  else{
    digitalWrite(internal_LED, HIGH); //turn on the led
  }
}
