#include <Arduino.h>
#include "Constants.c"

int windControl() {
  int i = 0;
  int j = 0;
  int turns = 0;
  unsigned long uptime = millis();


// s'arrete lorsque la roue a fait 1 tour complet
  while(millis() - uptime < WINDSPEED){
    //int BP = digitalRead(4);        // Lecture du capteur
    int BP = digitalRead(ANEMOMETER);        // Lecture du capteur

    if (BP == LOW) {
      digitalWrite(13, HIGH);      // Allume la Led
      i = 1;
    }else {
      digitalWrite(13, LOW);      // Eteind la Led
      j = 1;
    }


    // Compte le nombre de tour
    if(i == 1 && j == 1){
      turns += 1;
      i = 0;
      j = 0;
    }
  }
  return turns;
}
