#include "speed_bl.h"
uint8_t //编码器io定义 
Encoder_1_A_io=40,
Encoder_1_B_io=41,
Encoder_2_A_io=26,
Encoder_2_B_io=25;
hw_timer_t *timer0;
int FLAG_timIT = 0;
int Encoder_1_num,Encoder_2_num,Left_speed_option=50,Encoder_1_change,Left_pwm;
IRAM_ATTR void timer0_event(void)
{
    Encoder_1_change=Encoder_1_num;
    Left_pwm=Left_pwm+2*(Left_speed_option-Encoder_1_change);
    Left_motor_run(Left_pwm);
    Encoder_1_num=0;
    
}
void IRAM_ATTR Encoder_1_A_inter(void)
{
    if(digitalRead(Encoder_1_B_io))
    {
        Encoder_1_num++;
    }
    else
    {
        Encoder_1_num--;
    }
}
void IRAM_ATTR Encoder_2_A_inter(void)
{

}
void Encoder_init(void)
{
    pinMode(Encoder_1_A_io, INPUT_PULLDOWN);       //编码器引脚 输入模式
    pinMode(Encoder_1_B_io, INPUT_PULLDOWN);       //编码器引脚 输入模式
    pinMode(Encoder_2_A_io, INPUT_PULLDOWN);       //编码器引脚 输入模式
    pinMode(Encoder_2_B_io, INPUT_PULLDOWN);       //编码器引脚 输入模式
    attachInterrupt(Encoder_1_A_io,Encoder_1_A_inter,RISING);
    attachInterrupt(Encoder_2_A_io,Encoder_2_A_inter,RISING);
    
}
void timer0_init(void)
{
    timer0=timerBegin(0,80,true);
    timerAttachInterrupt(timer0,&timer0_event,true);
    timerAlarmWrite(timer0, 50000, true);
    timerAlarmEnable(timer0);
}
int Encoder_get_num(uint8_t Encoder_which)
{
    if(Encoder_which)
    {
        return Encoder_1_num;
    }else
    {
        return Encoder_2_num;
    }
}