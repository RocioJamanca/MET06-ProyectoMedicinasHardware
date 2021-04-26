#define debounceTime 200
unsigned long previousMillisButton = 0;   

ICACHE_RAM_ATTR void read_button(){
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillisButton >= debounceTime) {
    previousMillisButton = currentMillis;
    Serial.println("PANIC BUTTON PRESSED!");
  }
}

void setup_button(){
  pinMode(BUTTONPIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(BUTTONPIN), read_button, RISING);
}
