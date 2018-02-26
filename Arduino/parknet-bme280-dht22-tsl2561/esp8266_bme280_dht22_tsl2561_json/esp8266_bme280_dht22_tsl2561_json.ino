

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <DHT_U.h>
#include <DHT.h>
#include <Adafruit_TSL2561_U.h>
#include <ArduinoJson.h>
#include <time.h>

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


const char* ssid = "parknet";
const char* password = "s3ns0rNet";


ESP8266WebServer server(80);


#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 14
#define BME_CS 16
//#define SEALEVELPRESSURE_HPA (1013.25)
#define SEALEVELPRESSURE_HPA (1017.2)

#define DHTPIN 2     // what digital pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

DHT dht(DHTPIN, DHTTYPE);

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


// dht22 pre-flight
  
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




//bme pre-flight

    float humi = bme.readHumidity();
    float temp = bme.readTemperature();
    float dpc =  (temp - (14.55 + 0.114 * temp) * (1 - (0.01 * humi)) - pow(((2.5 + 0.007 * temp) * (1 - (0.01 * humi))),3) - (15.9 + 0.117 * temp) * pow((1 - (0.01 * humi)), 14));
    float dpf = ((dpc * 9)/5) + 32;


   

// tsl  
  sensors_event_t event;
   tsl.getEvent(&event);



// time 
time_t now = time(nullptr);




// get all the sensor data into a json object and server.send
/*
  StaticJsonBuffer<512> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();

    
    root["Fahrenheit-bme280"] = fahrenheit;
    root["Fahrenheit-dht22"] =  dht.readTemperature(true);
 
    root["Celsius-bme280"] = bme.readTemperature();
    root["Celsius-dht22"] = dht.readTemperature();
    
    root["Relative Humdity % - bme280"] = bme.readHumidity();
    root["Relative Humdity % - dht22"] = dht.readHumidity();

    root["Dew Point (bme280) Celsius"] = dpc;
    root["Dew Point (bme280) Fahrenheit"] = dpf;
    
    root["Heat Index (dht22) in F"] =  dht.computeHeatIndex(f, h);    
        
    root["Barometric Pressure in hPa"] = (bme.readPressure() / 100.0F);
    root["Approx. Altitude (meters)"] = (bme.readAltitude(SEALEVELPRESSURE_HPA));
    root["Approx. Altitude (feet)"] = (bme.readAltitude(SEALEVELPRESSURE_HPA)) * 3.3208399;
    
    root["Sea Level Pressure (noaa)"] =  SEALEVELPRESSURE_HPA;
    
    root["Lux"] = event.light;
*/


// let's try better json

   StaticJsonBuffer<512> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();

  
    root["sensor"] = "bme280";
    root["UTC"] = (ctime(&now));

    JsonArray& data = root.createNestedArray("data");
     data.add(fahrenheit);
     data.add(bme.readTemperature());
     data.add(bme.readHumidity());
     data.add(dpc);
     data.add(dpf);
     data.add((bme.readPressure() / 100.0F));
     data.add((bme.readAltitude(SEALEVELPRESSURE_HPA))) * 3.3208399;
     data.add(event.light);
  


    
  String readout;
  root.printTo(readout);
  server.send(200, "application/json", readout);

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

// ntp, really?
configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");
Serial.println("\nWaiting for time");
  while (!time(nullptr)) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("");
  while (!time(nullptr)) {
    Serial.print(".");
    delay(1000);
  }
Serial.println("");




  server.on("/",handleRoot);

    server.on("/inline", [](){
      server.send(200, "text/plain", "this works as well");
      });

    server.onNotFound(handleNotFound);

    server.begin();
    Serial.println("HTTP server started");
    delay(3000);
     time_t now = time(nullptr);
     Serial.println(ctime(&now));

    
  }

  void loop(void){
    server.handleClient();
 //   time_t now = time(nullptr);
 //   Serial.println(ctime(&now));
 //   delay(1000);

    
  }
