#include <WiFi.h>
#include "ThingSpeak.h"
#include <DHT.h>

const char* ssid = "";   // your network SSID (name) 
const char* password = ";   // your network password

WiFiClient client;

unsigned long myChannelNumber = 1;
const char* myWriteAPIKey = "";

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 30000;

// DHT sensor pin
const int dhtPin = 13;

// Create a sensor object
DHT dht(dhtPin, DHT22);

void setup() {
  Serial.begin(115200);  // Initialize serial
  dht.begin();
  
  WiFi.mode(WIFI_STA);   
  
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    
    // Connect or reconnect to WiFi
    if (WiFi.status() != WL_CONNECTED) {
      Serial.print("Attempting to connect");
      while (WiFi.status() != WL_CONNECTED) {
        WiFi.begin(ssid, password); 
        delay(5000);     
      } 
      Serial.println("\nConnected.");
    }

    // Get a new temperature reading
    float temperatureC = dht.readTemperature();
    Serial.print("Temperature (ºC): ");
    Serial.println(temperatureC);
    
    // Write to ThingSpeak
    int x = ThingSpeak.writeField(myChannelNumber, 1, temperatureC, myWriteAPIKey);

    if (x == 200) {
      Serial.println("Channel update successful.");
    } else {
      Serial.println("Problem updating channel. HTTP error code " + String(x));
    }
    lastTime = millis();
  }
}
