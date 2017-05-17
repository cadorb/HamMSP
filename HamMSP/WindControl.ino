#include <Arduino.h>
#include "Constants.c"

int windControl() {
  int i = 0;
  int j = 0;
  int turns = 0;
  int t = 0;


// s'arrete lorsque la roue a fait 1 tour complet
  while(turns < 10){
    //int BP = digitalRead(4);        // Lecture du capteur
    int BP = digitalRead(ANEMOMETER);        // Lecture du capteur

    if (BP == LOW) {
      digitalWrite(13, HIGH);      // Allume la Led
      i = 1;
    }else {
      digitalWrite(13, LOW);      // Eteind la Led
      j = 1;
    }


    //Serial.print(i);
    //Serial.print(" ");
    //Serial.println(j);


    // Compte le nombre de tour
    if(i == 1 && j == 1){
      turns += 1;
      Serial.print("Nombre de tour = ");
      Serial.println(turns);
      i = 0;
      j = 0;
    }

    t = t + LOOP_TIME; // temps passé pour faire 10 tours complets
    delay(LOOP_TIME);

    Serial.println(t);

    if(t > WINDSPEED*2){
      break;
    }
  }


// Si temps court : ya du vent
  if(t < WINDSPEED){
    Serial.print("t = ");
    Serial.println(t);
    Serial.println("Putain ya du vent !");
    return t;
  }else{
    return 0;
  }
}
