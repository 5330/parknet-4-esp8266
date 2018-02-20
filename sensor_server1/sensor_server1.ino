#include <ESP8266WiFiMesh.h>

#include <DHT_U.h>
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

// todo: insert this at run time but not in repo. 
const char* ssid = ".....";
const char* password = ".....";

ESP8266WebServer server(80);


#define DHTPIN 2     // what digital pin we're connected to

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);


/*

{
   float f = 1.123456789;
   char c[50]; //size of the number
    sprintf(c, "%g", f);
    printf(c);
    printf("\n");
}

*/



const int led = 13;

void handleRoot() {
  digitalWrite(led, 1);

#if 0
  //humidity
  float h = dht.readHumidity();
  char c[80];
  sprintf(c, "%g", h);

  //temp
  float t = dht.readTemperature(true);
  char temp[50];
  sprintf(temp, "%g", t);
  
  char readout[80];
  strcpy(readout, "Humidity: ");
  strcat(readout, c);
  strcat(readout, "% ");
  strcat(readout, " Temperature: ");
  strcat(readout, temp);
  strcat(readout, "F");
#endif

 char readout[80];
  
 sprintf(readout,
     "Humidity: %.02f%%  Temperature: %.02f F  Heat Index: %f",
     dht.readHumidity(),
     dht.readTemperature(true),
     0.0
     );


  
  //server.send(200, "text/plain", "Humidity =      hello from esp8266. this is where we want to put sensor data, ya?!");


/*
char str[80];
strcpy(str, "these ");
strcat(str, "strings ");
strcat(str, "are ");
strcat(str, "concatenated.");
    x = t c 

*/
    
   
   server.send(200, "text/plain", readout );
  
  digitalWrite(led, 0);
}

void handleNotFound(){
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void setup(void){
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/inline", [](){
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();
}
