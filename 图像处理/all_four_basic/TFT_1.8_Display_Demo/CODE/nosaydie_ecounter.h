/*
 * nosaydie_ecounter.h
 *
 *  Created on: 2021��11��12��
 *      Author: Administrator
 */

#ifndef CODE_NOSAYDIE_ECOUNTER_H_
#define CODE_NOSAYDIE_ECOUNTER_H_
#include "common.h"
#define ECOUNTE_LEFT_PIN1 GPT12_T2INB_P33_7     //���ֱ���������1
#define ECOUNTE_LEFT_PIN2 GPT12_T2EUDB_P33_6    //���ֱ���������2
#define ECOUNTE_RIGHT_PIN1 GPT12_T4INA_P02_8    //���ֱ���������1
#define ECOUNTE_RIGHT_PIN2 GPT12_T4EUDA_P00_9   //���ֱ���������2
void ecounter_init();   //��������ʼ��
void get_ecounter();  //��ȡ��\�����ٶ�


#endif /* CODE_NOSAYDIE_ECOUNTER_H_ */
