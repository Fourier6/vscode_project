#ifndef pid_kun_h
#define pid_kunh
#include "Arduino.h"
class pid_kun
{
private:
	float target_val;       //目标值
	float err;              //偏差值
	float err_last;         //上一个偏差值
    float err_last_last;    //上上一个偏差值
	float Kp,Ki,Kd;         //比例、积分、微分系数
	float integral;         //积分值
    float integral_max;     //积分最大值
    float integral_min;     //积分最小值
	float output_val;       //输出值
    float output_val_max;   //输出最大值
    float output_val_min;   //输出最小值
public:
    pid_kun(float Kp_temp,float Ki_temp,float Kd_temp,float target_val_temp,
    float integral_max_temp,float integral_min_temp,float output_val_max_temp,float output_val_min_temp);
    void set_pid(float Kp_temp,float Ki_temp,float Kd_temp);
    void set_integral_range(float integral_max_temp,float integral_min_temp);
    void set_target_val(float target_val_temp);
    void set_output_val_range(float output_val_max_temp,float output_val_min_temp);
    int PID_position(int current_val);

};
#endif