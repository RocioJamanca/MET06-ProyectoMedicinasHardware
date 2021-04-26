
ICACHE_RAM_ATTR void checkFall(){
  int upperSensor = digitalRead(PRESENCEPIN2);
  if(upperSensor == LOW){
    Serial.println("Fall detected");
  }
}

void setup_fallDetector(){
  pinMode(PRESENCEPIN1, INPUT);
  pinMode(PRESENCEPIN2, INPUT);
  attachInterrupt(digitalPinToInterrupt(PRESENCEPIN1), checkFall, RISING);
}
