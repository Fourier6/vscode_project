#include "./task_all/task_all.h"
TaskHandle_t motor_task;
QueueHandle_t motor_QueueHandle;
void motor_task_code(void * pvParameters)
{
  uint8_t zx=80;
  int speed_base=90;
  int zx_pid_out;
	float target_val=90;       //目标值
	float Kp=1.5,Ki=0.00,Kd=10;         //比例、积分、微分系数
  float integral_max=40;     //积分最大值
  float integral_min=-40;     //积分最小值
  float output_val_max=120;   //输出最大值
  float output_val_min=-120;   //输出最小值
  int sb;
  pid_kun zx_pid(Kp,Ki,Kd,target_val,integral_max,integral_min,output_val_max,output_val_min);
  motor_kun motor_left,motor_right;
  motor_left.begin(4,5,0);
  motor_right.begin(6,7,1);
  for( ;; )
  {
    vTaskDelay(20);
    xQueueReceive( serial2_QueueHandle, &zx,0);
    zx_pid_out=zx_pid.PID_position(zx);
    if(zx_pid_out>=0)
    {
      sb=zx_pid_out;
      xQueueOverwrite(motor_QueueHandle, &sb);
      motor_right.motor_run(speed_base-zx_pid_out/2);
      motor_left.motor_run(speed_base+zx_pid_out/2);
    }else if(zx_pid_out<0)
    {
      sb=zx_pid_out;
      xQueueOverwrite(motor_QueueHandle, &sb);
      motor_right.motor_run(speed_base-zx_pid_out/2);
      motor_left.motor_run(speed_base+zx_pid_out/2);
    }
    sb=zx_pid_out;
    
  }
}
