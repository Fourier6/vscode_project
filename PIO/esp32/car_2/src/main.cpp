#include <Arduino.h>
#include "../lib/motor_bl/motor_bl.h"
#include "../lib/speed_bl/speed_bl.h"
int num;
TaskHandle_t encoder_task;
TaskHandle_t other_task;
void encoder_task_code(void * pvParameters);
void other_task_code(void * pvParameters);
void setup() 
{
  Serial.begin(115200);
  // Serial2.begin(115200);
  // motor_init();
  // Encoder_init();
  // timer0_init();
  xTaskCreatePinnedToCore(encoder_task_code,"encoder",40000, NULL,2, &encoder_task,1);
  xTaskCreatePinnedToCore(other_task_code,"other",40000, NULL,1, &other_task,0);
  // xTaskCreatePinnedToCore(motor_task_code,"motor",10000, NULL,1, &motor_task,0); 
  // vTaskStartScheduler();
}

void loop() 
{

  // Left_motor_run(0);
}
void encoder_task_code(void * pvParameters)
{
  Encoder_init();
  while (1)
  {
    /* code */
    Serial.println(Encoder_1_change);
    vTaskDelay(200);
  }
  
}
void other_task_code(void * pvParameters)
{
  motor_init();
  timer0_init();
  while (1)
  {
    // Serial.println(Encoder_1_change);
    vTaskDelay(200);
  }
  
}