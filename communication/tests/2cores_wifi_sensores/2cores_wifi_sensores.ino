// Page: http://<meu_ip>

#include <WiFi.h>
#include <DallasTemperature.h>
#include <TrueRMSNew.h>

#define OPTICAL_PIN 23
#define DS18B20_pin 32

#define LPERIOD 100    // loop period time in us. In this case 100 us
#define VOLTAGE_PIN 36     // define the used ADC input channel
#define RMS_WINDOW 100   // rms window of 1667 samples, means 10 periods @60Hz

TaskHandle_t Task1;
TaskHandle_t Task2;

volatile bool led_on;
const TickType_t _1s = 1000 / portTICK_PERIOD_MS;
const TickType_t _250ms = 250 / portTICK_PERIOD_MS;
const TickType_t _1ms = 1 / portTICK_PERIOD_MS;
const TickType_t _100ms = 100 / portTICK_PERIOD_MS;
const TickType_t _10ms = 10 / portTICK_PERIOD_MS;
int n=0;

unsigned long nextLoop;
unsigned long last_time = 0;
int cnt=0;
float VoltRange = 3.30; // The full scale value is set to 5.00 Volts but can be changed when using an
                        // input scaling circuit in front of the ADC.

unsigned long dt_vel=0;
unsigned long timer_aux=0;

Rms readRms;

class SensorValues{
  public:
    SensorValues(unsigned short array_size, unsigned short factor){
      this->array_size = array_size;
      this->values = new int[array_size];
      this->factor = factor;
    }
    
    void add_value(float value) volatile{
      if(!is_reading){
        if(this->index < this->array_size){
          is_writing = 1;
          this->values[index] = value*factor;
          this->index++;
          is_writing = 0;
        }
      }
    }
    
    float get_value_at_index(short index){
      return this->values[index];
    }

    String publish_values() volatile{
      String str_values = "*/*";
      while(is_writing){
        //vTaskDelay( _1ms );
      }
      is_reading = 1;
      for(int i=0;i<this->array_size;i++){
        str_values += String(this->values[i]/float(this->factor)) + " ,";
        this->values[i]=0;
      }
      this->index = 0;
      is_reading = 0;
      return str_values;
    }
    volatile bool is_reading=0;
    volatile bool is_writing=1;
  private:
    volatile unsigned short index;
    volatile int *values;
    volatile unsigned short array_size;
    volatile unsigned short factor;
};

OneWire onewire(DS18B20_pin);
DallasTemperature sensores_temp(&onewire);

volatile SensorValues Temperatures = SensorValues(6,100);
volatile SensorValues Voltages = SensorValues(6,1000);

void setup() {
  Serial.begin(38400);
  //Serial.begin(115200);
  //while(!Serial) {}

  attachInterrupt(digitalPinToInterrupt(OPTICAL_PIN), get_delta, FALLING);

  xTaskCreatePinnedToCore(
      Task1code, /* Function to implement the task */
      "Task1", /* Name of the task */
      16384,  /* Stack size in words */
      //32768,  /* Stack size in words */
      NULL,  /* Task input parameter */
      5,  /* Priority of the task */
      &Task1,  /* Task handle. */
      0); /* Core where the task should run */

   pinMode(LED_BUILTIN, OUTPUT);
   sensores_temp.begin();
   readRms.begin(VoltRange, RMS_WINDOW, ADC_12BIT, BLR_ON, CNT_SCAN);
   readRms.start();
   nextLoop = micros() + LPERIOD;
   //digitalWrite(LED_BUILTIN,HIGH);
}

void loop() {
  vTaskDelay( _100ms );
  cycle();
  //yield();
  vTaskResume(Task1);
  
  UBaseType_t uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL );
  String mensagem = "Core:" + String((int)xPortGetCoreID()) + " --> Stack used: " + String((uint32_t)uxHighWaterMark);
  Serial.println(mensagem);
}

void Task1code( void * parameter) {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.print("Core: ");Serial.println(xPortGetCoreID());
  start_connection();
  for(;;) {
    run_client();
    vTaskDelay( _1s );
    UBaseType_t uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL );
    String mensagem = "Core:" + String((int)xPortGetCoreID()) + " --> Stack used: " + String((uint32_t)uxHighWaterMark);
    Serial.println(mensagem);
    //yield();
  }
}

unsigned long sum_rt=0;
unsigned int remaining_time = 0;
void cycle(){
  get_temperatures();
  get_voltages();
  get_rotations();

  digitalWrite(LED_BUILTIN,led_on);
  led_on = !led_on;
}

void delay_microseconds(uint16_t us)
{
  delayMicroseconds(us);
}

//void add_value(volatile int *values,float value,int index,int factor){
//  values[index] = value*factor;
//}

void get_temperatures(){
  sensores_temp.requestTemperatures();
  Temperatures.add_value(sensores_temp.getTempCByIndex(0));
}

void get_voltages(){
  for(int i=0;i<RMS_WINDOW;i++){
    int adcVal = analogRead(VOLTAGE_PIN); // read the ADC.
    readRms.update(adcVal);
    vTaskDelay( _1ms );
    //delay_microseconds(100);
  }
  readRms.publish();
  Voltages.add_value(540.0*readRms.rmsVal);
  //Serial.println(540.0*readRms.rmsVal);
}

void get_rotations(){
  int rotation = 0;
  if(dt_vel){
      rotation = 60000000/dt_vel;
      String mess = "Rotações [rpm]: " + String(rotation);
      Serial.println(mess);
  }
}

void get_delta(){   // interrupt
    dt_vel = micros() - timer_aux;                                       // Calcula delta de tempo entre ultimo pulso e instante atual
    timer_aux = micros();                                                // Atualiza instante do ultimo pulso
}
