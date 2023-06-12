#include "motor_bl.h"
uint8_t 
Left_motor_pwm_io=13,
Left_motor_direction_io=12,
Right_motor_pwm_io=19,
Right_motor_direction_io=21,
PWM_left_channel=0,
PWM_right_channel=1;
void motor_init() 
{
    pinMode(Left_motor_direction_io, OUTPUT);
    pinMode(Right_motor_direction_io, OUTPUT);
    digitalWrite(Left_motor_direction_io, LOW);
    digitalWrite(Right_motor_direction_io, LOW);
    ledcSetup(PWM_left_channel, 1000, 10);
    ledcAttachPin(Left_motor_pwm_io, PWM_left_channel);
    ledcWrite(PWM_left_channel,0);
    ledcSetup(PWM_right_channel, 1000, 10);
    ledcAttachPin(Right_motor_pwm_io, PWM_right_channel);
    ledcWrite(PWM_right_channel,0);
}
void Left_motor_pwm(uint32_t pwm_left)
{
    ledcWrite(PWM_left_channel,pwm_left);
}
void Right_motor_pwm(uint32_t pwm_right)
{
    ledcWrite(PWM_right_channel,pwm_right);
}
void Left_motor_run(int pwm_left)
{
    if(pwm_left>=0)
    {
        digitalWrite(Left_motor_direction_io, LOW);
        Left_motor_pwm(uint32_t( pwm_left));
    }
    else
    {
        digitalWrite(Left_motor_direction_io, HIGH);
        Left_motor_pwm(uint32_t( (1023+pwm_left)));
    }
}
void Right_motor_run(int pwm_right)
{
    if(pwm_right>=0)
    {
        digitalWrite(Right_motor_direction_io, LOW);
        Right_motor_pwm(uint32_t( pwm_right));
    }
    else
    {
        digitalWrite(Right_motor_direction_io, HIGH);
        Right_motor_pwm(uint32_t ((1023+pwm_right)));
    }
}