#include <Arduino.h>

void secure(){
int sensorState = digitalRead(7);
  Serial.println(sensorState);
  delay(100);
  if(sensorState == HIGH){
    digitalWrite(ledPin,HIGH);
    leds.setColorRGB(0, 10, 0, 0);
    Serial.println("BIIIIM");
    delay(5000);
  }else{
    digitalWrite(ledPin,LOW);
    leds.setColorRGB(0, 0, 10, 0);
  }
}
