/*
 * nosaydie_ecounter.h
 *
 *  Created on: 2021年11月12日
 *      Author: Administrator
 */

#ifndef CODE_NOSAYDIE_ECOUNTER_H_
#define CODE_NOSAYDIE_ECOUNTER_H_
#include "common.h"
#define ECOUNTE_LEFT_PIN1 GPT12_T2INB_P33_7     //左轮编码器引脚1
#define ECOUNTE_LEFT_PIN2 GPT12_T2EUDB_P33_6    //右轮编码器引脚2
#define ECOUNTE_RIGHT_PIN1 GPT12_T4INA_P02_8    //右轮编码器引脚1
#define ECOUNTE_RIGHT_PIN2 GPT12_T4EUDA_P00_9   //右轮编码器引脚2
void ecounter_init();   //编码器初始化
void get_ecounter();  //获取左\右轮速度


#endif /* CODE_NOSAYDIE_ECOUNTER_H_ */
