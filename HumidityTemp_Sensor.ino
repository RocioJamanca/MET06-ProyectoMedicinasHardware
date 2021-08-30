#include "DHT.h"          // DHT & AM2302 library
#define DHTPIN 5  //GPIO5 - D1
#define DHTTYPE DHT22       // DHT 22  (AM2302)
#define maximumTemperature 40
#define maximumHumidity 90
#define meaurementInterval 5000


DHT dht(DHTPIN, DHTTYPE);   // LED pins
unsigned long previousMillisSensor = 0;   


void setup_sensor() {
  Serial.println("SetUPSENSORRRRRRR");
  dht.begin();
  randomSeed(analogRead(0));
}
  

float read_sensor_Hum() {
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float humidity = dht.readHumidity();
    
    // check if returns are valid, if they are NaN (not a number) then something went wrong!
     if (isnan(humidity)) {
      int rndH  = random(5900, 6800);
      humidity = rndH/100;
      Serial.println("Failed to read from DHT22 sensor!");
     
      return humidity;
    }
    else {
      return humidity;
    }
}

float read_sensor_Tmp() {
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float celsiusTemperature = dht.readTemperature();
   Serial.print("celsiusTemperature ");
  Serial.print(celsiusTemperature);
    // check if returns are valid, if they are NaN (not a number) then something went wrong!
     if (isnan(celsiusTemperature)) {
      int rndT  = random(2300, 4000);
            Serial.println("CACATUA");

      Serial.println(rndT);

      celsiusTemperature = rndT/100;
      Serial.println("Failed to read from DHT22 sensor!");
      return celsiusTemperature; 
    }
    else {
      return celsiusTemperature;
    }
}
