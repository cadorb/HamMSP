#include <Arduino.h>
#include "Constants.c"

float windDirection(){
  int gyroValues[16] = [889,707,790,788,409,464,82,92,65,185,126,290,247,633,946,831]; // Resistances of the gyro
  float angleValues[16] = [0,22.5,45,67.5,90,112.5,135,157.5,180,202.5,225,247.5,270,292.5,315,337.5]; // Angles corresponding to resistances

  int windValue = analogRead(WEATHERCOCK);
  int windCardinal = findInArray(gyroValues, 16, *windValue);

  return windCardinal;
}


// Return indice of the found value
int findInArray (int array[], int sizeArray, int *value) {
    int i;
    for (i = 0; i < sizeArray; ++i) {
        array[i] == *value;
    }
        if (array[i]==*value) {
            return i;
        }
        else {
            return 20;
        }
}

/*

889		0
707		22.5
790		45
788		67.5
409		90
464		112.5
82		135
92		157.5
65		180
185		202.5
126		225
290		247.5
247		270
633		292.5
946		315
831		337.5

889
707
790
788
409
464
82
92
65
185
126
290
247
633
946
831

0
22.5
45
67.5
90
112.5
135
157.5
180
202.5
225
247.5
270
292.5
315
337.5

*/
