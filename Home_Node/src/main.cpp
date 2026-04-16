#include <Arduino.h>
#include <heltec.h>

#define Boot_Key 0
#define Led      35


void setup() {
  // put your setup code here, to run once:
  Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Disable*/, true /*Serial Enable*/);
  display.init();
  Serial.begin(115200);
  pinMode(Boot_Key, INPUT_PULLUP);
  pinMode(Led, OUTPUT);
 
}

void loop() {
  // put your main code here, to run repeatedly:
  display->clear();
  if(digitalRead(Boot_Key)==0) {
    digitalWrite(Led,HIGH);
    Serial.print("hello world !!! \n");
    delay(1000);
    digitalWrite(Led,LOW);
  }
  delay(100);
}