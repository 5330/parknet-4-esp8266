
/* i2c addressing
 *
 *  bme280: By default, the i2c address is 0x77.  If you add a jumper from SDO to GND, the address will change to 0x76.
 *
 *  tsl2561 Connect ADDR pin to ground to set the address to 0x29, connect it to 3.3V (vcc) to set the address to 0x49 or leave it floating (unconnected) to use address 0x39.
 *
 *  MPL3115A2 - I2C 7-bit fixed address 0x60
 *

*/



//esp8266
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Wire.h>
#include <SPI.h>


//sensors
#include <Adafruit_TSL2561_U.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_MPL3115A2.h>

#include <ArduinoJson.h>


/* need to store these as arduino macros.
https://github.com/RoboUlbricht/arduinoslovakia/tree/master/extra_parameter
http://www.arduinoslovakia.eu/blog/2017/6/vlozenie-definicie-makra-do-programu-v-arduine?lang=en
*/

const char* ssid = "parknet";
const char* password = "s3ns0rNet";
//const char* ssid = "Verizon-791L-A905";
//const char* password = "66888aa6";



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
//    while(1);
  }


  /* Display some basic information on this sensor */
  displaySensorDetails();

  /* Setup the sensor gain and integration time */
  configureSensor();

};





// default 1013.25  need to dynamically get this from http://www.wrh.noaa.gov/cnrfc/rsa_getObs.php?sid=KIND&num=48  updated every :54 of the hour
//#define SEALEVELPRESSURE_HPA (1013.25)
#define SEALEVELPRESSURE_HPA (1024.2)


Adafruit_BME280 bme; // I2C

Adafruit_MPL3115A2 baro = Adafruit_MPL3115A2();


const int led = 13;

void handleRoot() {

  digitalWrite(led, 1);

  // initialize sensor (required)
      if (!bme.begin()) {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
    //    while (1);
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


// MPL3115A2 pre-flight

if (! baro.begin()) {
  Serial.println("Couldnt find MPL3115A2");
//  while (1);
}
float pascals = baro.getPressure();
// Our weather page presents pressure in Inches (Hg)
// Use http://www.onlineconversion.com/pressure.htm for other units
//Serial.print(pascals/3377); Serial.println(" Inches (Hg)");

float altm = baro.getAltitude();
//Serial.print(altm); Serial.println(" meters");

float tempC = baro.getTemperature();
//Serial.print(tempC); Serial.println("*C");

float tempF = ((tempC * 9)/5) + 32;


// float wrangling. influxdb can't tolerate a float with no decimals so.. 

// Print in string the value of float with two decimal points


char pod002_bmeTempF[10];
sprintf(pod002_bmeTempF, "%.2f", (fahrenheit));

char pod002_bmeTempC[10];
sprintf(pod002_bmeTempC, "%.2f", (bme.readTemperature()));

char pod002_bmeHumidity[10];
sprintf(pod002_bmeHumidity, "%.2f", (bme.readHumidity()));

char pod002_bmeDewPointC[10];
sprintf(pod002_bmeDewPointC, "%.2f", (dpc));

char pod002_bmeDewPointF[10];
sprintf(pod002_bmeDewPointF, "%.2f", (dpf));

char pod002_bmePressurehPa[10];
sprintf(pod002_bmePressurehPa, "%.2f", ((bme.readPressure() / 100.0F)));

char pod002_bmeApproxAltitudeM[10];
sprintf(pod002_bmeApproxAltitudeM, "%.2f", ((bme.readAltitude(SEALEVELPRESSURE_HPA))));

char pod002_bmeApproxAltitudeF[10];
sprintf(pod002_bmeApproxAltitudeF, "%.2f", ((bme.readAltitude(SEALEVELPRESSURE_HPA)) * 3.3208399));

char pod002_tslLUX[10];
sprintf(pod002_tslLUX, "%.2f", (event.light));

char pod002_mplInchesHg[10];
sprintf(pod002_mplInchesHg, "%.2f", (pascals/3377));

char pod002_mplAltitudeMeters[10];
sprintf(pod002_mplAltitudeMeters, "%.2f", (baro.getAltitude()));

char pod002_mplAltitudeFeet[10];
sprintf(pod002_mplAltitudeFeet, "%.2f", (baro.getAltitude() * 3.3208399));

char pod002_mplTempC[10];
sprintf(pod002_mplTempC, "%.2f", (baro.getTemperature()));

char pod002_mplTempF[10];
sprintf(pod002_mplTempF, "%.2f", (tempF));


// json payload

  StaticJsonBuffer<1024> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();

    root["pod002_bmeTempF"] = (pod002_bmeTempF);
    root["pod002_bmeTempC"] = (pod002_bmeTempC);
    root["pod002_bmeHumidity"] = (pod002_bmeHumidity);
    root["pod002_bmeDewPointC"] = (pod002_bmeDewPointC);
    root["pod002_bmeDewPointF"] = (pod002_bmeDewPointF);
    root["pod002_bmePressurehPa"] = (pod002_bmePressurehPa);
    root["pod002_bmeApproxAltitudeM"] = (pod002_bmeApproxAltitudeM);
    root["pod002_bmeApproxAltitudeF"] = (pod002_bmeApproxAltitudeF);
    root["pod002_tslLUX"] = (pod002_tslLUX);
    root["pod002_mplInchesHg"] = (pod002_mplInchesHg);
    root["pod002_mplAltitudeMeters"] = (pod002_mplAltitudeMeters);
    root["pod002_mplAltitudeFeet"] = (pod002_mplAltitudeFeet);
    root["pod002_mplTempC"] = (pod002_mplTempC);
    root["pod002_mplTempF"] = (pod002_mplTempF);


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
