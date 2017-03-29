#include <Wire.h>

// Wifi - MQTT
#include <UnoWiFiDevEd.h>

// Temp pressure humidity
#include "Seeed_BME280.h"

// IMU
#include "I2Cdev.h"
#include "MPU9250.h"

// Constants
#include "Constants.c"

// LED
#include "ChainableLED.h"

// Init temp pressure humidity
BME280 bme280;

// Init led
ChainableLED leds(4, 5, NUM_LEDS);

const int ledPin=12;

void setup()
{
  Ciao.begin();
  Serial.begin(9600);
  if(!bme280.init()){
    Serial.println("BME280 error!");
  }
  leds.init();
  pinMode(ledPin,OUTPUT);
}

void loop()
{
  int sensorState = digitalRead(7);
  Serial.println(sensorState);
  delay(100);
  if(sensorState == HIGH){
    digitalWrite(ledPin,HIGH);
    leds.setColorRGB(0, 255, 0, 0);
    Serial.println("BIIIIM");
    getBME280();
    delay(5000);
  }else{
    digitalWrite(ledPin,LOW);
    leds.setColorRGB(0, 0, 0, 0);
  }

  
  
  

  delay(100);
}
