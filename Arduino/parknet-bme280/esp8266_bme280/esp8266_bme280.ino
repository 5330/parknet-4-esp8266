

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

// guest network at 5330, move these secrets out.
const char* ssid = "parknet";
const char* password = "s3ns0rNet";


ESP8266WebServer server(80);


#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 14
#define BME_CS 16
#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

const int led = 13;

void handleRoot() {
  digitalWrite(led, 1);

// initialize sensor (required)
      if (!bme.begin()) {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        while (1);
      }


 char readout[180];


 float celsius = bme.readTemperature();

 float fahrenheit = ((celsius * 9)/5) + 32;



 sprintf(readout,
     "BME280 - Relative Humidity: %.02f%% Temp: %.02fc %.02ff Pressure: %.02f hPa",
      bme.readHumidity(),
      bme.readTemperature(),
      fahrenheit,
      (bme.readPressure() / 100.0F)
      );


  server.send(200, "text/plain",readout );

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
