/*
 * nosaydie_ecounter.c
 *
 *  Created on: 2021��11��12��
 *      Author: Administrator
 */
#include "nosaydie_ecounter.h"
#include "headfile.h"
#include "common.h"
int16 speedl;   //���ֵ�ǰ�ٶ�
int16 speedr;   //���ֵ�ǰ�ٶ�
extern struct PID Left_ctr;            //���ֿ������
extern struct PID Right_ctr;           //���ֿ������
void ecounter_init()
{
    gpt12_init(GPT12_T2, ECOUNTE_LEFT_PIN1, ECOUNTE_LEFT_PIN2);     //���������ʼ��
    gpt12_init(GPT12_T4, ECOUNTE_RIGHT_PIN1, ECOUNTE_RIGHT_PIN2);   //�ұ�������ʼ��
}
void get_ecounter()
{
    speedl = gpt12_get(GPT12_T2);
    speedr = -1 * gpt12_get(GPT12_T4);
    Left_ctr.currspeed = speedl;
    Right_ctr.currspeed = speedr;
    gpt12_clear(GPT12_T2);
    gpt12_clear(GPT12_T4);
}
