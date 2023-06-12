#include "task_all.h"

TaskHandle_t serial_task;
QueueHandle_t serial2_QueueHandle;
SemaphoreHandle_t serialMutex;
void Serial_println(const char* buffer)
{
  if(xSemaphoreTake(serialMutex, portMAX_DELAY) == pdTRUE)
  {
    Serial.println(buffer);
    xSemaphoreGive(serialMutex);
  }
}

void serial_task_code(void * pvParameters)
{
  Serial.begin(115200);
  Serial2.begin(115200);
  Serial2.onReceive(serial2_Event); 
  int c;
  while (1)
  {

    if( (xQueueReceive( motor_QueueHandle, &c, 0 ) == pdPASS )&&(xSemaphoreTake(serialMutex, portMAX_DELAY) == pdTRUE))
		{
			Serial.println(c);
       xSemaphoreGive(serialMutex);
		}
    vTaskDelay(100);
  }  
}
void IRAM_ATTR serial2_Event(void) {
  while (Serial2.available()) {
    uint8_t c = Serial2.read();
    xQueueOverwrite(serial2_QueueHandle, &c);
    // Serial.println(c); 
  }
}
