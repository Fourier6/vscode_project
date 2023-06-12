#include "speed_bl.h"
uint8_t //编码器io定义 
Encoder_1_A_io=14,
Encoder_1_B_io=27,
Encoder_2_A_io=26,
Encoder_2_B_io=25;
hw_timer_t *timer0;
int FLAG_timIT = 0;
int Encoder_1_num,Encoder_2_num,Left_speed_option=150,Encoder_1_change,Left_pwm;
typedef struct PID_1
{
	float target_val=150;   
	float err;          
	float err_last;    
    float err_last_last;
	float Kp=5,Ki=5,Kd=5;     
	float integral;     
	float output_val;   
}PID_SP;
PID_SP Speed_pid;

IRAM_ATTR void timer0_event(void)
{
    Encoder_1_change=Encoder_1_num;
    Encoder_1_num=0;
    Speed_pid.err=Speed_pid.target_val-Encoder_1_change;
    // Left_pwm=Left_pwm+2*(Left_speed_option-Encoder_1_change);
    // Speed_pid.output_val
    Speed_pid.integral+=Speed_pid.Ki*Speed_pid.err;
    if(Speed_pid.integral>1000)
    {
        Speed_pid.integral=1000;
    }else if(Speed_pid.integral<-1000)
    {
        Speed_pid.integral=-1000;
    }
    Speed_pid.output_val=Speed_pid.err*Speed_pid.Kp+(Speed_pid.err-Speed_pid.err_last)*Speed_pid.Kd+Speed_pid.integral;
    if(Speed_pid.output_val>1000)
    {
        Speed_pid.output_val=1000;
    }else if(Speed_pid.output_val<-1000)
    {
        Speed_pid.output_val=-1000;
    }
    Left_motor_run(Speed_pid.output_val);
    Speed_pid.err_last=Speed_pid.err;
    // Serial.println(Encoder_1_num);
}
IRAM_ATTR void Encoder_1_inter(void)
{
    if(digitalRead(Encoder_1_B_io))
    {
        if(digitalRead(Encoder_1_B_io))
        {
            Encoder_1_num++;
        }else
        {
            Encoder_1_num--;
        }
    }
    else
    {
        if(digitalRead(Encoder_1_B_io))
        {
            Encoder_1_num--;
        }else
        {
            Encoder_1_num++;
        }
    }
}
// IRAM_ATTR void Encoder_1_B_inter(void)
// {
//     if(digitalRead(Encoder_1_B_io)==digitalRead(Encoder_1_A_io))
//     {
//         Encoder_1_num++;
//     }
//     else
//     {
//         Encoder_1_num--;
//     }
// }
void Encoder_init(void)
{
    pinMode(Encoder_1_A_io, INPUT_PULLDOWN);       //编码器引脚 输入模式
    pinMode(Encoder_1_B_io, INPUT_PULLDOWN);       //编码器引脚 输入模式
    pinMode(Encoder_2_A_io, INPUT_PULLDOWN);       //编码器引脚 输入模式
    pinMode(Encoder_2_B_io, INPUT_PULLDOWN);       //编码器引脚 输入模式
    attachInterrupt(Encoder_1_A_io,Encoder_1_inter,CHANGE);
    attachInterrupt(Encoder_2_B_io,Encoder_1_inter,CHANGE);
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