const char* ssid     = "LUCAS";
const char* password = "23030828laba";

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
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println();
          client.println("<!DOCTYPE HTML><html><head>");
          client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"></head>");
          client.println("<h1>ESP32 - Web Server example</h1>");
          client.println("<p>LED <a href=\"on\"><button>ON</button></a>&nbsp;<a href=\"off\"><button>OFF</button></a></p>");
          client.println("</html>");
          break;
        }
        if (c == '\n') 
        {
          //print_buffer(linebuf);
          // you're starting a new line
          currentLineIsBlank = true;
          if (strstr(linebuf,"GET /on") > 0)
          {
            Serial.println("========= ON =========");
            digitalWrite(LED_BUILTIN, HIGH);
          }
          else if (strstr(linebuf,"GET /off") > 0)
          {
            Serial.println("======== OFF =========");
            digitalWrite(LED_BUILTIN, LOW);
          }

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
