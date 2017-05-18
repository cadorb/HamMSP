#include <Arduino.h>
// Multithread simulator
#include <SoftTimer.h>
#include <PciListener.h>
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


void meteoCheck(Task* me){
  leds.setColorRGB(0, 25, 25, 0);
  Serial.println("#Checking meteo ");
  getBME280();
}

void windDirCheck(Task* me){
  leds.setColorRGB(0, 25, 25, 25);
  Serial.print("##Checking wind direction = ");
  Serial.println(windDirection());
}

void windSpeedCheck(Task* me){
  Serial.println("####Checking wind speed");
  windStatus = windControl();

  if(windStatus > 10){
    leds.setColorRGB(0, 255, 0, 0);
    Serial.println("Le vent est fort");
    Serial.print("Sens du vent : ");
    Serial.println(windDirection());
    Ciao.write(CONNECTOR, TOPIC_WINDSPEED, String(windStatus)); // pushes data into a channel
    Ciao.write(CONNECTOR, TOPIC_WIND, String(windDirection())); // pushes data into a channel
  }else{
    Serial.println("Le vent est faible");
    leds.setColorRGB(0, 0, 15, 0);
  }
}

// -- meteoCheck will be launched on every 2 seconds.
Task taskMeteo(30000, meteoCheck);
// -- windDirCheck will be launched on every 3 seconds.
Task taskWindDir(10000, windDirCheck);
// -- windSpeedCheck will be launched on every 5000 milliseconds.
Task taskWindSpeed(WINDSPEED, windSpeedCheck);


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


  // -- Register the tasks to the timer manager. Both tasks will start immediately.
  SoftTimer.add(&taskMeteo);
  SoftTimer.add(&taskWindDir);
  SoftTimer.add(&taskWindSpeed);
}
