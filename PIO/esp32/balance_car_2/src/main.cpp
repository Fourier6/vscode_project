#include <Arduino.h>

int num;
void setup() 
{
  Serial.begin(115200);
  Serial1.begin(115200,SERIAL_8N1, 7, 15);
  // motor_init();
  // Encoder_init();
  // timer0_init();

}

void loop() 
{
  delay(500);
  Serial.println("Encoder_1_change");
  Serial1.println("ok");
}
