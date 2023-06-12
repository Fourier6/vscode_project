#ifndef task_mpu6500_h
#define task_mpu6500_h
#include <Arduino.h>
#include <MPU6500_WE.h>
extern TaskHandle_t Task_mpu6500;
void task_mpu6500 (void *pvParameters);
extern float pitch;
#endif