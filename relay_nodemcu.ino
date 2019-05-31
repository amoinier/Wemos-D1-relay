#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

const char *ssid = "";
const char *password = "";
ESP8266WebServer server(80);

int relay = 5;
volatile byte relayState = OUTPUT;

void setup(void) {
  Serial.begin (115200);

  // Pin for relay module set as output
  pinMode(relay, relayState);
  pinMode(relay, INPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }

  Serial.print("\nConnected ");
  Serial.println(WiFi.localIP());

  server.on("/toggle", toggle);
  server.onNotFound(handleNotFound);
  server.begin();
 
  delay(50);
  Serial.println("HTTP server started"); 
}

void loop(void) {
  server.handleClient();
}

void toggle(void) {
  responseReq();
  setRelay();
}

void setRelay() {
  Serial.println("Set relay");
  Serial.print(relayState, DEC);
  if (relayState == OUTPUT) {
    relayState = INPUT;
  }
  else if (relayState == INPUT) {
    relayState = OUTPUT;
  }
  pinMode(relay, relayState);
  Serial.print(relayState, DEC);
}

void handleNotFound(void) {
  String message = "URL Not Found\n\n";

  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for ( uint8_t i = 0; i < server.args(); i++ )
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";

  server.send(404, "text/plain", message);
}

void responseReq(void) {
  char temp[5000];

  snprintf(temp, 5000,
  "<!DOCTYPE html>\n<html>\n\
  <head>\n\
    <title>NeoPixel</title>\n\
    <meta name=\"viewport\" content=\"width=device-width, height=device-height, initial-scale=1.0, user-scalable=0, minimum-scale=1.0, maximum-scale=1.0\" />\n\
  </head>\n\
  <body>\n\
  </body>\
  </html>\n"
  );
  server.send(200, "text/html", temp);
}
