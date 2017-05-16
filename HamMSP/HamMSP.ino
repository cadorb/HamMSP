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
  Serial.begin(9600);
  delay(1000);
  Serial.println("Starting Arduino");

  Ciao.begin();
  if(!bme280.init()){
    Serial.println("BME280 error!");
  }
  leds.init();
  leds.setColorRGB(0, 0, 15, 0);
  pinMode(ledPin,OUTPUT);
}

void loop()
{
  Serial.println("Getting BME");
  getBME280();

  leds.setColorRGB(0, 0, 15, 0);
  Serial.println("Checking Wind");
  windStatus = windControl();

  if(windStatus != 0){
    leds.setColorRGB(0, 255, 0, 0);
    Serial.println("Le vent est fort");
    Serial.print("Sens du vent : ");
    Serial.println(windDirection());
    Ciao.write(CONNECTOR, TOPIC_WIND, String(windDirection())); // pushes data into a channel
    delay(5000);
  }else{
    Serial.println("Le vent est faible");
    leds.setColorRGB(0, 0, 15, 0);
  }

  delay(100);
}
