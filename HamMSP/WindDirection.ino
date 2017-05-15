#include <Arduino.h>
#include "Constants.c"

int windDirection(){
  return analogRead(WEATHERCOCK);
}
