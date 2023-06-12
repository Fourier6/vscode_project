#include <Arduino.h>

// put function declarations here:

void setup()
{

  pinMode(PC13,OUTPUT);
}

void loop()
{
  // put your main cod
  digitalToggle(PC13);
  delay(1000);
}
// put function definitions here:

