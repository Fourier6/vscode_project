#ifndef task_all_h
#define task_all_h
#include <Arduino.h>
#include "../lib/motor_kun/motor_kun.h"
#include "../lib/pid_kun/pid_kun.h"
#include "../lib/webserve_kun/webserve_kun.h"
#include "HardwareSerial.h"  
#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
void motor_task_code(void * pvParameters);

void serial_task_code(void * pvParameters);

void webserve_task_code(void * pvParameters);
void IRAM_ATTR serial2_Event(void);
void Serial_println(const char* buffer);

extern TaskHandle_t serial_task;
extern TaskHandle_t motor_task;
extern TaskHandle_t webserve_task;

extern QueueHandle_t serial2_QueueHandle;
extern QueueHandle_t motor_QueueHandle;
extern QueueHandle_t webserve_QueueHandle;

extern SemaphoreHandle_t serialMutex;



#endif