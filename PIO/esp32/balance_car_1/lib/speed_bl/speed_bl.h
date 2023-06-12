#ifndef speed_bl_h
#define speed_bl_h
#include "Arduino.h"
#include "motor_bl.h"
void Encoder_init(void);
void IRAM_ATTR timer0_event(void);
void timer0_init(void);
void IRAM_ATTR Encoder_1_A_inter(void);
void IRAM_ATTR Encoder_2_A_inter(void);
int Encoder_get_num(uint8_t Encoder_which);
extern int Encoder_1_change;
#endif