// Page: http://<meu_ip>

#include <WiFi.h>
#include <DallasTemperature.h>

#define DS18B20_pin 33

TaskHandle_t Task1;
TaskHandle_t Task2;

volatile bool led_on;
const TickType_t _1s = 1000 / portTICK_PERIOD_MS;
const TickType_t _250ms = 250 / portTICK_PERIOD_MS;
const TickType_t _1ms = 1 / portTICK_PERIOD_MS;
const TickType_t _100ms = 100 / portTICK_PERIOD_MS;
int n=0;

class SensorValues{
  public:
    SensorValues(unsigned short array_size, unsigned short factor){
      this->array_size = array_size;
      this->values = new int[array_size];
      this->factor = factor;
    }
    
    void add_value(float value){
      if(this->index < this->array_size && !is_reading){
        is_writing = 1;
        this->values[index] = value*factor;
        this->index++;
        is_writing = 0;
      }
    }
    
    float get_value_at_index(short index){
      return this->values[index];
    }

    String publish_values(){
      String str_values = "*/*";
      while(is_writing);
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
    unsigned short index;
    volatile int *values;
    unsigned short array_size;
    unsigned short factor;
};

OneWire onewire(DS18B20_pin);
DallasTemperature sensores_temp(&onewire);

SensorValues Temperatures = SensorValues(6,100);

void setup() {
  Serial.begin(38400);
  //Serial.begin(115200);
  //while(!Serial) {}

  xTaskCreatePinnedToCore(
      Task1code, /* Function to implement the task */
      "Task1", /* Name of the task */
      16384,  /* Stack size in words */
      //131072,  /* Stack size in words */
      NULL,  /* Task input parameter */
      5,  /* Priority of the task */
      &Task1,  /* Task handle. */
      0); /* Core where the task should run */

   pinMode(LED_BUILTIN, OUTPUT);
   sensores_temp.begin();
}

void loop() {
  vTaskDelay( _1s/2 );
  cycle();
//  UBaseType_t uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL );
//  Serial.println(uxHighWaterMark);
}

void Task1code( void * parameter) {
  //pinMode(LED_BUILTIN, OUTPUT);
  Serial.print("Core: ");Serial.println(xPortGetCoreID());
  start_connection();
  for(;;) {
    run_client();
    vTaskDelay( _1s );
    yield();
  }
}

unsigned long sum_rt=0;
unsigned int remaining_time = 0;
void cycle(){
  sensores_temp.requestTemperatures();
  Temperatures.add_value(sensores_temp.getTempCByIndex(0));
  digitalWrite(LED_BUILTIN,led_on);
}

void delay_microseconds(uint16_t us)
{
  delayMicroseconds(us);
}

void add_value(volatile int *values,float value,int index,int factor){
  values[index] = value*factor;
}
