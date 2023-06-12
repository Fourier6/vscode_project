#include <Arduino.h>
#include "./task_kun/task_all.h"


void setup() 
{
  Serial.begin(115200);
  creat_all_task();
}

void loop() 
{

  vTaskDelay(100);
}