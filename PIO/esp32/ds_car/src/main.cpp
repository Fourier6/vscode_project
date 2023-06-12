#include <Arduino.h>
// #include "../lib/motor_kun/motor_kun.h"
// #include "../lib/pid_kun/pid_kun.h"
// #include "../lib/webserve_kun/webserve_kun.h"
// #include "HardwareSerial.h"  
// #include <ArduinoJson.h>
// #include <ESPAsyncWebServer.h>
#include "./task_all/task_all.h"


void setup() {
  serialMutex = xSemaphoreCreateMutex();

  serial2_QueueHandle = xQueueCreate( 1, sizeof( uint8_t ) );
  motor_QueueHandle = xQueueCreate( 1, sizeof( int ) );
  webserve_QueueHandle =xQueueCreate( 50, sizeof( char ) );


  xTaskCreatePinnedToCore(motor_task_code,"motor",10000, NULL,15, &motor_task,1);
  xTaskCreatePinnedToCore(serial_task_code,"serial",10000, NULL,1,&serial_task,0);
  xTaskCreatePinnedToCore(webserve_task_code,"webserve",10000, NULL,1,&webserve_task,0);
  // vTaskStartScheduler();
}

void loop() {
  vTaskDelay(100);
}





