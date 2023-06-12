#ifndef motor_bl_h
#define motor_bl_h
#include "Arduino.h"
void motor_init();
void Left_motor_run(int pwm_left);
void Right_motor_run(int pwm_right);
#endif