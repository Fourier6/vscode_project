/*
 * nosaydie_pwmcontrol.c
 *
 *  Created on: 2021年11月12日
 *      Author: Administrator
 */
#include "nosaydie_pwmcontrol.h"
#include "headfile.h"
#include "common.h"
void PWM_init()
{
    //初始化PWM引脚
    gtm_pwm_init(MOTOR1_PWM, 10000, 0);
    gtm_pwm_init(MOTOR2_PWM, 10000, 0);
    //初始化方向控制引脚
    gpio_init(MOTOR1_DIR, GPO, 0, PUSHPULL);
    gpio_init(MOTOR2_DIR, GPO, 0, PUSHPULL);
}
//右电机赋值
void RPWM_set(int32 speed1_power)
{
    if(0<=speed1_power) //电机1   正转 设置占空比为 百分之 (1000/GTM_ATOM0_PWM_DUTY_MAX*100)
    {
        pwm_duty(MOTOR1_PWM, speed1_power);
        gpio_set(MOTOR1_DIR, 0);
    }
    else                //电机1   反转
    {
        pwm_duty(MOTOR1_PWM, -speed1_power);
        gpio_set(MOTOR1_DIR, 1);
    }
}
//左电机赋值
void LPWM_set(int32 speed2_power)
{
    if(0<=speed2_power) //电机2   正转
    {
        pwm_duty(MOTOR2_PWM, speed2_power);
        gpio_set(MOTOR2_DIR, 0);
    }
    else                //电机2   反转
    {
        pwm_duty(MOTOR2_PWM, -speed2_power);
        gpio_set(MOTOR2_DIR, 1);
    }}
