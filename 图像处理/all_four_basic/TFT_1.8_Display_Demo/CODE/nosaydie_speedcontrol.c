/*
 * nosaydie_speedcontrol.c
 *
 *  Created on: 2021��11��12��
 *      Author: Administrator
 */
#include "nosaydie_speedcontrol.h"
#include "headfile.h"
#include "common.h"
extern int16 speedl;
extern int16 speedr;
extern struct PID Left_ctr;            //���ֿ������
extern struct PID Right_ctr;           //���ֿ������
void speed_control()
{
    //������PID�ṹ�����ٶȸ�ֵ
    Left_ctr.currspeed = speedl;
    Right_ctr.currspeed = speedr;
    //��ȡĿ���ٶ�
    Left_ctr.goalspeed = 40;
    Right_ctr.goalspeed = 40;
    //p i dʵ��
    pid_realize_l_speed();
    pid_realize_r_speed();
    //p w m��ֵ
    //Right_ctr.out = -2000;    //����������
    //Left_ctr.out = 2000;
    LPWM_set(-(int32)Left_ctr.out);
    RPWM_set(-(int32)Right_ctr.out);
}
