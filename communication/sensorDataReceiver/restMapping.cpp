#include "restMapping.h"

WiFiClient client;
HTTPClient http;
extern const char* serverName;

int postData(String json) {

    // Your Domain name with URL path or IP address with path
    http.begin(client, serverName);

    // Specify content-type header
    http.addHeader("Content-Type", "application/json");
    // Send HTTP POST request
    int httpResponseCode = http.POST(json);
    // Free resources
    http.end();

    return httpResponseCode;

}
