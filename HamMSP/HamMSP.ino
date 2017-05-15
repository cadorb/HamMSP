#include <Arduino.h>
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
ChainableLED leds(2,3, NUM_LEDS);

const int ledPin=12;

int windStatus;


void setup()
{
  Ciao.begin();
  Serial.begin(9600);
  if(!bme280.init()){
    Serial.println("BME280 error!");
  }
  leds.init();
  leds.setColorRGB(0, 0, 15, 0);
  pinMode(ledPin,OUTPUT);
}

void loop()
{
  getBME280();
  windStatus = windControl();

  if(windStatus != 0){
    leds.setColorRGB(0, 255, 0, 0);
    Serial.println("Le vent est fort");
    Serial.print("Sens du vent : ");
    Serial.println(windDirection());
  }else{
    Serial.println("Le vent est faible");
    leds.setColorRGB(0, 0, 15, 0);
  }

  delay(100);
}
