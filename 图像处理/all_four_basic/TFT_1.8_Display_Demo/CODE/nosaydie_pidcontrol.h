/*
 * nosaydie_pidcontrol.h
 *
 *  Created on: 2021��11��12��
 *      Author: Administrator
 */

#ifndef CODE_NOSAYDIE_PIDCONTROL_H_
#define CODE_NOSAYDIE_PIDCONTROL_H_
#include "common.h"
struct PID
{
        float kp;
        float ki;
        float kd;
        int16 goalspeed;
        int16 currspeed;
        float out;
};
struct PID_moshi
{
        float val_target;   //Ŀ��ֵ
        float val_now;      //��ǰֵ
        float error_now;    //ƫ��
        float error_last;   //��һ��ƫ��
        float kp,ki,kd;     //p\i\d��ֵ
        float kp_out,ki_out,kd_out;
        float integral;     //����ֵ
        float output_val;   //���ֵ
        float last_out;
};
//��غ���
void pid_init();            //p i d��ʼ������
void pid_realize_l_speed(); //�����ٶ�p i dʵ���㷨
void pid_realize_r_speed(); //�����ٶ�p i dʵ���㷨
#endif /* CODE_NOSAYDIE_PIDCONTROL_H_ */
