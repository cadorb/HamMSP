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

void meteoCheck(Task* me);
void windSpeedCheck(Task* me);
void windDirCheck(Task* me);


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
  Serial.println(String(bme280.getTemperature()));
  leds.init();
  leds.setColorRGB(0, 0, 15, 0);
  pinMode(ledPin,OUTPUT);


  // -- Register the tasks to the timer manager. Both tasks will start immediately.
  SoftTimer.add(&taskMeteo);
  SoftTimer.add(&taskWindDir);
  SoftTimer.add(&taskWindSpeed);
}



void meteoCheck(Task* me){
  leds.setColorRGB(0, 25, 25, 0);
  Serial.print("#Checking meteo ");
  float temperature = bme280.getTemperature();
  Serial.println(temperature);
  Serial.println(String(temperature));
  Ciao.write(CONNECTOR, TOPIC_TEMP, String(temperature)); // pushes data into a channel
  Ciao.write(CONNECTOR, TOPIC_PRES, String(bme280.getPressure()));
  Ciao.write(CONNECTOR, TOPIC_HUMI, String(bme280.getHumidity()));
}

void windDirCheck(Task* me){
  leds.setColorRGB(0, 25, 25, 25);
  Serial.print("##Checking wind direction = ");
  Serial.println(windDirection());
}

void windSpeedCheck(Task* me){
  Serial.println("####Checking wind speed");
  windStatus = windControl();

  if(windStatus > 50){
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
