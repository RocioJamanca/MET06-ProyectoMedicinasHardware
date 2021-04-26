// Main

// PINS
#define DHTPIN 5  //GPIO5 - D1
#define BUTTONPIN 2 //GPIO2 - D4 
#define PRESENCEPIN1 0  //GPIO0 - D3  n //LowerSensor - RED
#define PRESENCEPIN2 14 //GPIO14 - D5 //UpperSensor - GREEN
// SENSOR CONSTANTS
#define DHTTYPE DHT22       // DHT 22  (AM2302)
#define maximumTemperature 40
#define maximumHumidity 90
#define meaurementInterval 5000

void setup (){
  // Setup serial monitor
  Serial.begin(9600);
  setup_sensor();
  setup_button();
  setup_fallDetector();
}

void loop(){
  read_sensor();
 
}
