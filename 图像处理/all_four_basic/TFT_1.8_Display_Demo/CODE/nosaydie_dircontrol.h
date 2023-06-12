/*
 * nosaydie_dircontrol.h
 *
 *  Created on: 2021年11月12日
 *      Author: Administrator
 */

#ifndef CODE_NOSAYDIE_DIRCONTROL_H_
#define CODE_NOSAYDIE_DIRCONTROL_H_
#include "common.h"
//引脚配置
#define S_MOTOR_PIN   ATOM1_CH1_P33_9       //定义舵机引脚
//相关函数
void smotor_init();     //舵机初始化
void GetCDFTable();     //生成CDF表
void get_mid_bias();    //获取中线偏差
void get_pure_bias();   //获取追踪偏差
void MidBias_FitTurnR();//由图像中线偏差拟合半径并得到预期转向角速度
float GetTargetTurnAngleSpeed(float car_speed, float steer_R);//由车速和转向半径计算预期转向角速度
void ServoTurnControl();//舵机转向控制
void Servo_Duty(int32 duty);//利用占空比来控制舵机
float mysin(float ang); //计算正弦值

#endif /* CODE_NOSAYDIE_DIRCONTROL_H_ */
