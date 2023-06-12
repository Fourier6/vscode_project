/*
 * nosaydie_pwmcontrol.c
 *
 *  Created on: 2021��11��12��
 *      Author: Administrator
 */
#include "nosaydie_pwmcontrol.h"
#include "headfile.h"
#include "common.h"
void PWM_init()
{
    //��ʼ��PWM����
    gtm_pwm_init(MOTOR1_PWM, 10000, 0);
    gtm_pwm_init(MOTOR2_PWM, 10000, 0);
    //��ʼ�������������
    gpio_init(MOTOR1_DIR, GPO, 0, PUSHPULL);
    gpio_init(MOTOR2_DIR, GPO, 0, PUSHPULL);
}
//�ҵ����ֵ
void RPWM_set(int32 speed1_power)
{
    if(0<=speed1_power) //���1   ��ת ����ռ�ձ�Ϊ �ٷ�֮ (1000/GTM_ATOM0_PWM_DUTY_MAX*100)
    {
        pwm_duty(MOTOR1_PWM, speed1_power);
        gpio_set(MOTOR1_DIR, 0);
    }
    else                //���1   ��ת
    {
        pwm_duty(MOTOR1_PWM, -speed1_power);
        gpio_set(MOTOR1_DIR, 1);
    }
}
//������ֵ
void LPWM_set(int32 speed2_power)
{
    if(0<=speed2_power) //���2   ��ת
    {
        pwm_duty(MOTOR2_PWM, speed2_power);
        gpio_set(MOTOR2_DIR, 0);
    }
    else                //���2   ��ת
    {
        pwm_duty(MOTOR2_PWM, -speed2_power);
        gpio_set(MOTOR2_DIR, 1);
    }}
