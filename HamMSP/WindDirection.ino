#include <Arduino.h>
#include "Constants.c"


float windDirection(){
  int gyroValues[16] = {889,707,790,788,409,464,82,92,65,185,126,290,247,633,946,831}; // Resistances of the gyro
  float angleValues[17] = {0,22.5,45,67.5,90,112.5,135,157.5,180,202.5,225,247.5,270,292.5,315,337.5,360}; // Angles corresponding to resistances
  int indice = 17;
  float windCardinal;

  int windValue = analogRead(WEATHERCOCK);

  for (int i = 0; i < 16; ++i) {
    if (windValue <= gyroValues[i] + 3 && windValue >= gyroValues[i] - 3) {
      indice = i;
  }
}

  windCardinal = angleValues[indice];
  return windCardinal;
}
