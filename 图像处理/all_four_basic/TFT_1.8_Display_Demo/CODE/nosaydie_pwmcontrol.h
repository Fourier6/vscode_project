/*
 * nosaydie_pwmcontrol.h
 *
 *  Created on: 2021年11月12日
 *      Author: Administrator
 */

#ifndef CODE_NOSAYDIE_PWMCONTROL_H_
#define CODE_NOSAYDIE_PWMCONTROL_H_
#include "common.h"
#define MOTOR1_DIR   P21_2              //定义1电机方向控制引脚
#define MOTOR1_PWM   ATOM0_CH1_P21_3    //定义1电机PWM引脚

#define MOTOR2_DIR   P21_4              //定义2电机方向控制引脚
#define MOTOR2_PWM   ATOM0_CH3_P21_5    //定义2电机PWM引脚
//电机初始化
void PWM_init();
//电机赋值
void LPWM_set(int32 speed_power);
void RPWM_set(int32 speed_power);
#endif /* CODE_NOSAYDIE_PWMCONTROL_H_ */
