


//esp8266
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Wire.h>
#include <SPI.h>


//sensors
//#include <DHT_U.h>
//#include <DHT.h>
#include <Adafruit_TSL2561_U.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <ArduinoJson.h>

//#include <time.h>
//ntp not yet impemented. maybe try https://www.arduinoslovakia.eu/blog/2017/7/esp8266---ntp-klient-a-letny-cas?lang=en
// or better yet, fuck it and write time to "db" when we log. 
//#include <WiFiUdp.h>
//#include <TimeLib.h>
//#include <Timezone.h>


/* need to store these as arduino macros. 
https://github.com/RoboUlbricht/arduinoslovakia/tree/master/extra_parameter
http://www.arduinoslovakia.eu/blog/2017/6/vlozenie-definicie-makra-do-programu-v-arduine?lang=en
*/
//const char* ssid = "parknet";
//const char* password = "s3ns0rNet";
const char* ssid = "Verizon-791L-A905";
const char* password = "66888aa6";



ESP8266WebServer server(80);

/*
 * Connect SCL to I2C SCL Clock
 * Connect SDA to I2C SDA Data
*/
Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);

void displaySensorDetails(void)
{
  sensor_t sensor;
  tsl.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" lux");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" lux");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" lux");
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

/**************************************************************************/
/*
    Configures the gain and integration time for the TSL2561
*/
/**************************************************************************/
void configureSensor(void)
{
  /* You can also manually set the gain or enable auto-gain support */
  // tsl.setGain(TSL2561_GAIN_1X);      /* No gain ... use in bright light to avoid sensor saturation */
  // tsl.setGain(TSL2561_GAIN_16X);     /* 16x gain ... use in low light to boost sensitivity */
  tsl.enableAutoRange(true);            /* Auto-gain ... switches automatically between 1x and 16x */

  /* Changing the integration time gives you better sensor resolution (402ms = 16-bit data) */
  // tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);      /* fast but low resolution */
  // tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_101MS);  /* medium resolution and speed   */
  tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_402MS);  /* 16-bit data but slowest conversions */


/* Initialise the sensor */
  //use tsl.begin() to default to Wire,
  //tsl.begin(&Wire2) directs api to use Wire2, etc.
  if(!tsl.begin())
  {
    /* There was a problem detecting the TSL2561 ... check your connections */
    Serial.print("Ooops, no TSL2561 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }


  /* Display some basic information on this sensor */
  displaySensorDetails();

  /* Setup the sensor gain and integration time */
  configureSensor();

};


// define bme280
#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 14
#define BME_CS 16

// default 1013.25  need to dynamically get this from http://www.wrh.noaa.gov/cnrfc/rsa_getObs.php?sid=KIND&num=48  updated every :54 of the hour
//#define SEALEVELPRESSURE_HPA (1013.25)
#define SEALEVELPRESSURE_HPA (1008.9)


//define dht22
//#define DHTPIN 2     // what digital pin we're connected to
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//DHT dht(DHTPIN, DHTTYPE);

Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI
//Adafruit_BME280 bme; // I2C

const int led = 13;

void handleRoot() {

  digitalWrite(led, 1);

  // initialize sensor (required)
      if (!bme.begin()) {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        while (1);
      }



//bme pre-flight

    float humi = bme.readHumidity();
    float temp = bme.readTemperature();
    float dpc =  (temp - (14.55 + 0.114 * temp) * (1 - (0.01 * humi)) - pow(((2.5 + 0.007 * temp) * (1 - (0.01 * humi))),3) - (15.9 + 0.117 * temp) * pow((1 - (0.01 * humi)), 14));
    float dpf = ((dpc * 9)/5) + 32;
    
    float fahrenheit = ((temp * 9)/5) + 32;

   

// tsl pre-flight
  sensors_event_t event;
   tsl.getEvent(&event);
 


// time 
//time_t now = time(nullptr);
// i swear this was reporting accurate time but 8 hours ahead, now it is showing epoch/1970 shit, fekkit. 


  
  
// json payload
 
  StaticJsonBuffer<1024> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
 
 //   root["reading"] = (ctime(&now));
    root["bmeTempF"] = (fahrenheit);
    root["bmeTempC"] = (bme.readTemperature());
    root["bmeHumidity"] = (bme.readHumidity());
    root["bmeDewPointC"] = (dpc);
    root["bmeDewPointF"] = (dpf);
    root["bmePressurehPa"] = ((bme.readPressure() / 100.0F));
    root["bmeApproxAltitudeM"] = ((bme.readAltitude(SEALEVELPRESSURE_HPA)));
    root["bmeApproxAltitudeF"] = ((bme.readAltitude(SEALEVELPRESSURE_HPA)) * 3.3208399);
    root["tslLux"] = (event.light);

 
  String readout;
  root.prettyPrintTo(readout);
  server.send(200, "text/json", readout );

  digitalWrite(led, 0);

}


/*  text readout  this works

 char readout[1024];

 float celsius = bme.readTemperature();
 float fahrenheit = ((celsius * 9)/5) + 32;

 float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }


  sensors_event_t event;
  tsl.getEvent(&event);


 sprintf(readout,
     "BME280 \n Relative Humidity: %.02f%% \n Temp: %.02fc / %.02ff \n Pressure: %.02f hPa \n Approx. Altitude: %.02f meters / %.02f feet \nDHT22 \n Relative Humidity: %.02f%% \n Temp: %.02fc / %.02ff \n Heat Index: %.02ff \nTSL2561 \n lux: %.02f lux \n",
      bme.readHumidity(),
      bme.readTemperature(),
      fahrenheit,
      (bme.readPressure() / 100.0F),
      (bme.readAltitude(SEALEVELPRESSURE_HPA)),
      (bme.readAltitude(SEALEVELPRESSURE_HPA)) * 3.2808399,
       dht.readHumidity(),
       dht.readTemperature(),
       dht.readTemperature(true),
       dht.computeHeatIndex(f, h),
       event.light
      );
 server.send(200, "text/plain", readout );

  digitalWrite(led, 0);
}

*/








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

    server.on("/inline", [](){
      server.send(200, "text/plain", "this works as well");
      });

    server.onNotFound(handleNotFound);

    server.begin();
    Serial.println("HTTP server started");
 //   time_t now = time(nullptr);
//     Serial.println(ctime(&now));
  }

  void loop(void){
    server.handleClient();
  }
