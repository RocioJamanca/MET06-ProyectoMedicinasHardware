// Fall Detector, check if someone has fallen
#define PRESENCEPIN1 0  //GPIO0 - D3  n //LowerSensor - RED
#define PRESENCEPIN2 14 //GPIO14 - D5 //UpperSensor - GREEN

/*
ICACHE_RAM_ATTR void checkFall2(){
  int upperSensor = digitalRead(PRESENCEPIN2);
  if(upperSensor == LOW){
    Serial.println("Fall detected");

  }
}
*/
void setup_fallDetector(){
/*
  pinMode(PRESENCEPIN1, INPUT);
  pinMode(PRESENCEPIN2, INPUT);*/
  //attachInterrupt(digitalPinToInterrupt(PRESENCEPIN1), checkFall2, RISING);
}
