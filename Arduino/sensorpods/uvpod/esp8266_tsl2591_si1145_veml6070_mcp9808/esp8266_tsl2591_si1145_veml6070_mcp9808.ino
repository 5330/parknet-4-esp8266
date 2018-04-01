
/* i2c addressing
 *
 *  bme280: By default, the i2c address is 0x77.  If you add a jumper from SDO to GND, the address will change to 0x76.
 *  tsl2561 Connect ADDR pin to ground to set the address to 0x29, connect it to 3.3V (vcc) to set the address to 0x49 or leave it floating (unconnected) to use address 0x39.

*/



//esp8266
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Wire.h>
#include <SPI.h>



//sensors
#include <Adafruit_Sensor.h>
#include "Adafruit_TSL2591.h"


#include <ArduinoJson.h>



/* need to store these as arduino macros.
https://github.com/RoboUlbricht/arduinoslovakia/tree/master/extra_parameter
http://www.arduinoslovakia.eu/blog/2017/6/vlozenie-definicie-makra-do-programu-v-arduine?lang=en
*/

const char* ssid = "parknet";
const char* password = "s3ns0rNet";
//const char* ssid = "Verizon-791L-A905";
//const char* password = "66888aa6";

const int led = 13;


ESP8266WebServer server(80);

Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591); // pass in a number for the sensor identifier (for your use later)


/**************************************************************************/
/*
    Configures the gain and integration time for the TSL2591
*/
/**************************************************************************/
void configureSensor(void)
{
  // You can change the gain on the fly, to adapt to brighter/dimmer light situations
  //tsl.setGain(TSL2591_GAIN_LOW);    // 1x gain (bright light)
  tsl.setGain(TSL2591_GAIN_MED);      // 25x gain
  //tsl.setGain(TSL2591_GAIN_HIGH);   // 428x gain

  // Changing the integration time gives you a longer time over which to sense light
  // longer timelines are slower, but are good in very low light situtations!
  //tsl.setTiming(TSL2591_INTEGRATIONTIME_100MS);  // shortest integration time (bright light)
  // tsl.setTiming(TSL2591_INTEGRATIONTIME_200MS);
  tsl.setTiming(TSL2591_INTEGRATIONTIME_300MS);
  // tsl.setTiming(TSL2591_INTEGRATIONTIME_400MS);
  // tsl.setTiming(TSL2591_INTEGRATIONTIME_500MS);
  // tsl.setTiming(TSL2591_INTEGRATIONTIME_600MS);  // longest integration time (dim light)

  /* Display the gain and integration time for reference sake */
  Serial.println(F("------------------------------------"));
  Serial.print  (F("Gain:         "));
  tsl2591Gain_t gain = tsl.getGain();
  switch(gain)
  {
    case TSL2591_GAIN_LOW:
      Serial.println(F("1x (Low)"));
      break;
    case TSL2591_GAIN_MED:
      Serial.println(F("25x (Medium)"));
      break;
    case TSL2591_GAIN_HIGH:
      Serial.println(F("428x (High)"));
      break;
    case TSL2591_GAIN_MAX:
      Serial.println(F("9876x (Max)"));
      break;
  }
  Serial.print  (F("Timing:       "));
  Serial.print((tsl.getTiming() + 1) * 100, DEC);
  Serial.println(F(" ms"));
  Serial.println(F("------------------------------------"));
  Serial.println(F(""));


if(!tsl.begin())
{
  /* There was a problem detecting the TSL2561 ... check your connections */
  Serial.print("Ooops, no TSL2591 detected ... Check your wiring or I2C ADDR!");
//    while(1);
}


  /* Configure the sensor */
  configureSensor();

  // Now we're ready to get readings ... move on to loop()!

// more sensors here
};




//  main loop
void handleRoot() {

  digitalWrite(led, 1);



// tsl pre-flight
/* Get a new sensor event */
sensors_event_t event;
tsl.getEvent(&event);


// json payload

  StaticJsonBuffer<1024> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();


    root["uvpod001_LUX"] = (event.light);



  String readout;
  root.prettyPrintTo(readout);
  server.send(200, "text/json", readout );

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

  server.on("/",handleRoot);

    server.onNotFound(handleNotFound);
    server.begin();
    Serial.println("HTTP server started");
  }

  void loop(void){
    server.handleClient();
  }
  
