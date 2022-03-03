// http://192.168.15.3/H turns the LED on
// http://192.168.15.3/L turns it off

#include <WiFi.h>

TaskHandle_t Task1;
TaskHandle_t Task2;

unsigned long t_c0 = 0;
unsigned long loop_time = 100;
unsigned long T_c0 = 0;
const TickType_t xDelay = 1000 / portTICK_PERIOD_MS;
const TickType_t shortDelay = 1 / portTICK_PERIOD_MS;
const TickType_t serverDelay = 100 / portTICK_PERIOD_MS;
int n=0;

int a=0;
volatile bool led_on = 0;
//
//unsigned long IRAM_ATTR microssec()
//{
//    return (unsigned long) (esp_timer_get_time());
//}
//
//void IRAM_ATTR delayMicros(uint32_t us)
//{
//    uint32_t m = microssec();
//    if(us){
//        uint32_t e = (m + us);
//        if(m > e){ //overflow
//            while(microssec() > e){
//                NOP();
//            }
//        }
//        while(microssec() < e){
//            NOP();
//        }
//    }
//}

void setup() {
  Serial.begin(38400);
  //Serial.begin(115200);
  //while(!Serial) {}

  xTaskCreatePinnedToCore(
      Task1code, /* Function to implement the task */
      "Task1", /* Name of the task */
      16384,  /* Stack size in words */
      NULL,  /* Task input parameter */
      5,  /* Priority of the task */
      &Task1,  /* Task handle. */
      0); /* Core where the task should run */

   t_c0 = micros();

//    xTaskCreatePinnedToCore(
//      Task2code, /* Function to implement the task */
//      "Task2", /* Name of the task */
//      1000,  /* Stack size in words */
//      NULL,  /* Task input parameter */
//      5,  /* Priority of the task */
//      &Task2,  /* Task handle. */
//      1); /* Core where the task should run */
}

void loop() {
  // put your main code here, to run repeatedly:
  //int a = analogRead(A0);
  //vTaskDelay( xDelay );
  vTaskDelay( 3*shortDelay );
  for(int i=0;i<100;i++){
    cycle();
  }
}

//void loop(){
//  //vTaskDelay(portMAX_DELAY);
//}
//
//void Task2code( void * parameter) {
//  for(;;){
//    //int a = analogRead(A0);
//    //vTaskDelay( xDelay );
//    //yield();
//    //Serial.println(a);
//    cycle();
//  }
//}

void Task1code( void * parameter) {
//  const char* ssid     = "LUCAS";
//  const char* password = "23030828laba";
//  
//  WiFiServer server(80);
//  
//  const int led =  LED_BUILTIN;      // the number of the LED pin
//  
//  // Client variables 
//  char linebuf[80];
//  int charcount=0;
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.print("Core: ");Serial.println(xPortGetCoreID());
  start_connection();
  for(;;) {
    run_client();
    vTaskDelay( xDelay );
    yield();
  }
}

unsigned long sum_rt=0;
unsigned int remaining_time = 0;
void cycle(){
  //t_c0 += loop_time;
    n++;
    if(n==10000){
    //if(n==100){
//      Serial.print("Core: ");
//      Serial.print(xPortGetCoreID());
//      Serial.print("\t us: ");
//      Serial.println(float(micros()-T_c0)/n);
//      Serial.println(float(sum_rt)/n);
      T_c0 = micros();
      n=0;
      sum_rt=0;
    }
    //unsigned long remaining_time = loop_time - microssec();
    //delayMicroseconds(80);
    remaining_time = loop_time - (micros()-t_c0);
    sum_rt += remaining_time;
    //delayMicroseconds(remaining_time);
    delay_microseconds(remaining_time);
//    Serial.println(remaining_time);
//    delay(10000);
    
    //delayMicroseconds(100);
    //ets_delay_us(remaining_time);
    //delayMicros(remaining_time);
    //while(micros() - t_c0 < loop_time);
    //vTaskDelay(shortDelay);
    t_c0 = micros();
    //t_c0 = microssec();
}

void delay_microseconds(uint16_t us)
{
  delayMicroseconds(us);
}
