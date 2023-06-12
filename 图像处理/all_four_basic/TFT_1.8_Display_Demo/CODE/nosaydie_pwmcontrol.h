/*
 * nosaydie_pwmcontrol.h
 *
 *  Created on: 2021��11��12��
 *      Author: Administrator
 */

#ifndef CODE_NOSAYDIE_PWMCONTROL_H_
#define CODE_NOSAYDIE_PWMCONTROL_H_
#include "common.h"
#define MOTOR1_DIR   P21_2              //����1��������������
#define MOTOR1_PWM   ATOM0_CH1_P21_3    //����1���PWM����

#define MOTOR2_DIR   P21_4              //����2��������������
#define MOTOR2_PWM   ATOM0_CH3_P21_5    //����2���PWM����
//�����ʼ��
void PWM_init();
//�����ֵ
void LPWM_set(int32 speed_power);
void RPWM_set(int32 speed_power);
#endif /* CODE_NOSAYDIE_PWMCONTROL_H_ */
