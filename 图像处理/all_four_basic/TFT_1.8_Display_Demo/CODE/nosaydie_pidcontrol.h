/*
 * nosaydie_pidcontrol.h
 *
 *  Created on: 2021年11月12日
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
        float val_target;   //目标值
        float val_now;      //当前值
        float error_now;    //偏差
        float error_last;   //上一个偏差
        float kp,ki,kd;     //p\i\d的值
        float kp_out,ki_out,kd_out;
        float integral;     //积分值
        float output_val;   //输出值
        float last_out;
};
//相关函数
void pid_init();            //p i d初始化函数
void pid_realize_l_speed(); //左轮速度p i d实现算法
void pid_realize_r_speed(); //右轮速度p i d实现算法
#endif /* CODE_NOSAYDIE_PIDCONTROL_H_ */
