#include "task_motor.h"
typedef struct {
float P;
float I;
float D;
}pid_dat;
extern float pitch;
extern QueueHandle_t pid_queue;
TaskHandle_t Task_motor;
motor_kun motor_l;
motor_kun motor_r;
pid_data_kun pid_data;
pid_kun pid_l(5,2,3,1000,-1000,0,500,-500);
pid_kun pid_r(5,2,3,1000,-1000,0,500,-500);
pid_kun Pitch(0,0,0,80,-80,0,40,-40);
void task_motor (void *pvParameters)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(10); // 任务执行周期为1秒
    pid_dat Pid;
    encoder.begin(16,15,2,1);
    motor_l.begin(37,38,0);
    motor_r.begin(45,47,1);
    int speed_l=0,speed_r=0;
    int pwm_l=0,pwm_r=0;
    int flag=0;
    float wz;
    while (1)
    {
        if(xQueueReceive(pid_queue, &Pid, 0)==pdPASS)
        {
            Pitch.set_pid(Pid.P,Pid.I,Pid.D);
            Serial.printf("P=%f,I=%f,D=%f",Pid.P,Pid.I,Pid.D);
        }
        wz=-1*Pitch.PID_position(pitch,0);
        speed_l=encoder.get_speed(1);
        speed_r=encoder.get_speed(2);
        // pwm_l=pid_l.PID_increment(speed_l,wz);
        // pwm_r=pid_r.PID_increment(speed_r,wz);
        pwm_l=-wz*10;
        pwm_r=-wz*10;
        motor_l.motor_run(pwm_l);    
        motor_r.motor_run(pwm_r);
        if(flag==30)
        {
            Serial.print("L");
            Serial.print(pwm_l);
            Serial.print("  ");
            Serial.print(speed_l);
            Serial.print("  ");
            Serial.print("R");
            Serial.print(pwm_r);
            Serial.print("  ");
            Serial.print(speed_r);
            Serial.print("Pitch ");
            Serial.println(pitch);
            flag=0;
        }
        flag++;
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
    
}