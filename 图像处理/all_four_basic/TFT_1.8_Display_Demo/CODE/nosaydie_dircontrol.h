/*
 * nosaydie_dircontrol.h
 *
 *  Created on: 2021��11��12��
 *      Author: Administrator
 */

#ifndef CODE_NOSAYDIE_DIRCONTROL_H_
#define CODE_NOSAYDIE_DIRCONTROL_H_
#include "common.h"
//��������
#define S_MOTOR_PIN   ATOM1_CH1_P33_9       //����������
//��غ���
void smotor_init();     //�����ʼ��
void GetCDFTable();     //����CDF��
void get_mid_bias();    //��ȡ����ƫ��
void get_pure_bias();   //��ȡ׷��ƫ��
void MidBias_FitTurnR();//��ͼ������ƫ����ϰ뾶���õ�Ԥ��ת����ٶ�
float GetTargetTurnAngleSpeed(float car_speed, float steer_R);//�ɳ��ٺ�ת��뾶����Ԥ��ת����ٶ�
void ServoTurnControl();//���ת�����
void Servo_Duty(int32 duty);//����ռ�ձ������ƶ��
float mysin(float ang); //��������ֵ

#endif /* CODE_NOSAYDIE_DIRCONTROL_H_ */
