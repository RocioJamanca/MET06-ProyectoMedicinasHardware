#include "DHT.h"          // DHT & AM2302 library

DHT dht(DHTPIN, DHTTYPE);   // LED pins
unsigned long previousMillisSensor = 0;   


void setup_sensor() {
  dht.begin();
}
  

void read_sensor() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillisSensor >= meaurementInterval) {
    previousMillisSensor = currentMillis;

    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float humidity = dht.readHumidity();
    float celsiusTemperature = dht.readTemperature();
    float fahrenheitTemperature = dht.readTemperature(true);
  
    // check if returns are valid, if they are NaN (not a number) then something went wrong!
     if (isnan(humidity) || isnan(celsiusTemperature) || 
        isnan(fahrenheitTemperature)) {
      Serial.println("Failed to read from DHT22 sensor!");
      return;
    }
    else {
      if(celsiusTemperature > maximumTemperature){
        Serial.print("Alert: High Temperature ");
        Serial.print(celsiusTemperature);
        Serial.println(" C");
      }
  
      if(humidity > maximumHumidity){
        Serial.print("Alert: High Humidity ");
        Serial.print(humidity);
        Serial.println(" %");
      }
    }
  }

}
