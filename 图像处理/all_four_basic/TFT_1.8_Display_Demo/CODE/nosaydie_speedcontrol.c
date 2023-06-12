/*
 * nosaydie_speedcontrol.c
 *
 *  Created on: 2021年11月12日
 *      Author: Administrator
 */
#include "nosaydie_speedcontrol.h"
#include "headfile.h"
#include "common.h"
extern int16 speedl;
extern int16 speedr;
extern struct PID Left_ctr;            //左轮控制相关
extern struct PID Right_ctr;           //右轮控制相关
void speed_control()
{
    //左右轮PID结构体现速度赋值
    Left_ctr.currspeed = speedl;
    Right_ctr.currspeed = speedr;
    //获取目标速度
    Left_ctr.goalspeed = 40;
    Right_ctr.goalspeed = 40;
    //p i d实现
    pid_realize_l_speed();
    pid_realize_r_speed();
    //p w m赋值
    //Right_ctr.out = -2000;    //调试引脚用
    //Left_ctr.out = 2000;
    LPWM_set(-(int32)Left_ctr.out);
    RPWM_set(-(int32)Right_ctr.out);
}
