#ifndef task_all_h
#define task_all_h
#include <Arduino.h>
#include "task_mpu6500.h"
#include "task_motor.h"
#include "task_web.h"
extern QueueHandle_t pid_queue;
void creat_all_task(void);
#endif