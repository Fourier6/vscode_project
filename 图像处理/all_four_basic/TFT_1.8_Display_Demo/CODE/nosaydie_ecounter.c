/*
 * nosaydie_ecounter.c
 *
 *  Created on: 2021年11月12日
 *      Author: Administrator
 */
#include "nosaydie_ecounter.h"
#include "headfile.h"
#include "common.h"
int16 speedl;   //左轮当前速度
int16 speedr;   //右轮当前速度
extern struct PID Left_ctr;            //左轮控制相关
extern struct PID Right_ctr;           //右轮控制相关
void ecounter_init()
{
    gpt12_init(GPT12_T2, ECOUNTE_LEFT_PIN1, ECOUNTE_LEFT_PIN2);     //左编码器初始化
    gpt12_init(GPT12_T4, ECOUNTE_RIGHT_PIN1, ECOUNTE_RIGHT_PIN2);   //右编码器初始化
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
