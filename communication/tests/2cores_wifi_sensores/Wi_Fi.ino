//const char* ssid     = "LUCAS";
//const char* password = "23030828laba";
const char* ssid     = "NEUMLINK";
const char* password = "joiak3ler";

WiFiServer server(80);

//const int led =  LED_BUILTIN;      // the number of the LED pin

// Client variables 
char linebuf[80];
int charcount=0;

//void print_buffer(char* buff){
//  String msg;
//  for(int i=0;i<80;i++){
//    char c = buff[i];
//    msg += c;
//  }
//  Serial.println(msg);
//}

void start_connection(){
   // We start by connecting to a WiFi network
   Serial.println("Start");
   WiFi.begin(ssid, password);
   yield();
  //attempt to connect to Wifi network:
  Serial.println("Trying connection...");
  while(WiFi.status() != WL_CONNECTED) 
  {
    // Connect to WPA/WPA2 network.
    vTaskDelay(serverDelay);
    yield();
  }
  Serial.print("Status: ");Serial.println(WiFi.status());
  server.begin();
  Serial.println(WiFi.localIP());
}

void run_client(){
  // listen for incoming clients
  WiFiClient client = server.available();
  Serial.print("+");
  if (client) 
  {
    Serial.println("New client");
    memset(linebuf,0,sizeof(linebuf));
    charcount=0;
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) 
    {
      if (client.available()) 
      {
        char c = client.read();
        //Serial.write(c);
        //read char by char HTTP request
        linebuf[charcount]=c;
        if (charcount<sizeof(linebuf)-1) charcount++;

        if (c == '\n' && currentLineIsBlank) 
        {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          // the connection will be closed after completion of the response
          client.println("Refresh: 5"); // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html lang=\"en\">");
          client.println("<head>");
          client.println("<meta charset=\"utf-8\">");
          client.println("<title>cactus.io</title>");
          client.println("</head>");
          client.println("<body>");
          // output the value from the DS18B20 temperature sensor
//          client.print("Temperature is ");
//          client.print(temp);
//          client.println(" &#8451;"); // change #8451 to #8457 for fahrenheit
          printTemperatures(client);
          client.println("</body>");
          client.println("</html>");
          break;
        }
        if (c == '\n') 
        {
//          Serial.print("analog Read: ");
//          Serial.println(analog);
          // you're starting a new line
          currentLineIsBlank = true;
//          if (strstr(linebuf,"GET /y") > 0)
//          {
//            Serial.println("========= ON =========");
//            digitalWrite(LED_BUILTIN, HIGH);
//          }
//          else if (strstr(linebuf,"GET /n") > 0)
//          {
//            Serial.println("======== OFF =========");
//            digitalWrite(LED_BUILTIN, LOW);
//          }

          // you're starting a new line
          currentLineIsBlank = true;
          memset(linebuf,0,sizeof(linebuf));
          charcount=0;
        } 
        else if (c != '\r') 
        {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    //delay(1);
    vTaskDelay(shortDelay);

    // close the connection:
    client.stop();
  }
}

void printTemperatures(WiFiClient client){
  client.println("Temperatures:");
  String str_values = Temperatures.flush_values();
  //Serial.println(str_values);
  client.println(str_values);
//  client.println(Temperatures.flush_values());
//  for(int i=0; i<temps_index;i++){
//    client.print(temps[i]/float(temp_factor));
//    client.println(" &#8451; , "); // change #8451 to #8457 for fahrenheit
//  }
//  temps_index=0;
}
