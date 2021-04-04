/*
  433 MHz RF Module Transmitter Demonstration 2
  RF-Xmit-Demo-2.ino
  Demonstrates 433 MHz RF Transmitter Module with DHT-22 Sensor
  Use with Receiver Demonstration 2
  DroneBot Workshop 2018
  https://dronebotworkshop.com
*/
// Include RadioHead Amplitude Shift Keying Library
#include <RH_ASK.h>
// Include dependant SPI Library 
#include <SPI.h> 

// Include DHT Libraries from Adafruit
// Dependant upon Adafruit_Sensors Library
#include "DHT.h";

// Define Constants
#define DHTPIN 7       // DHT-22 Output Pin connection
#define DHTTYPE DHT11   // DHT Type is DHT 22 (AM2302)

// Define Variables
float hum;    // Stores humidity value in percent
float temp;   // Stores temperature value in Celcius

// Define output strings
const String str_TransmitID = "Poele"; //Poele
const String sep = ":";
String str_humid;
String str_temp;
String str_out;

// Create Amplitude Shift Keying Object
RH_ASK rf_driver;

// Initialize DHT sensor for normal 16mhz Arduino
DHT dht(DHTPIN, DHTTYPE);

void setup() {

  // Initialize ASK Object
  rf_driver.init();
  // Start DHT Sensor
  dht.begin();
  Serial.begin(9600);

}
void loop()
{
    delay(1000);  // Delay so DHT-22 sensor can stabilize
               //original delay 500
    hum = dht.readHumidity();  // Get Humidity value
    temp= dht.readTemperature();  // Get Temperature value
    
    // Convert Humidity to string
    str_humid = String(hum);
    
    // Convert Temperature to string
    str_temp = String(temp);

    // Combine Humidity and Temperature
    //Update 29 april 29 2020
    //COM PROTOCOL --- ID:HUMIDITY:TEMPERATURE
    str_out = str_TransmitID+sep+ str_humid +sep+str_temp+sep;  
    // Compose output character
    static char *msg = str_out.c_str();
    
    rf_driver.send((uint8_t *)msg, strlen(msg));
    rf_driver.waitPacketSent();
    
    Serial.println(strlen(msg));
    Serial.println(str_out);
    //mySwitch.send(13981443,24);//(((ADD)))
    //delay(6000); 
}
