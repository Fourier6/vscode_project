#ifndef task_web_h
#define task_web_h
#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoJson.h>
extern TaskHandle_t Task_web;
typedef struct {
float P;
float I;
float D;
} pid_dat;
void task_web(void *pvParameters);
#endif