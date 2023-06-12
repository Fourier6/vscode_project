#ifndef task_motor_h
#define task_motor_h
#include <Arduino.h>
#include "../lib/encoder_kun/encoder_kun.h"
#include "../lib/motor_kun/motor_kun.h"
#include "../lib/pid_kun/pid_kun.h"
extern TaskHandle_t Task_motor;
void task_motor(void *pvParameters);

#endif