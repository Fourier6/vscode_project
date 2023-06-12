#include "nosaydie_dircontrol.h"
#include "headfile.h"
#include "common.h"
//��ر���
//  �ⲿ����
//      1.ȫ��
extern uint8 Image_W;
extern uint8 Image_H;
//      2.����
extern struct LEFT_EDGE L_edge[150];
extern struct RIGHT_EDGE R_edge[150];
extern struct LEFT_EDGE *Mid_Line;
extern uint8 left_findflag;
extern uint8 right_findflag;
extern uint8 L_edge_count;
extern uint8 R_edge_count;
extern uint8 L_edge_end_row;
extern uint8 R_edge_end_row;
//      3.����
extern uint8 Mid_count;
extern uint8 M_edge_end_row;
//      4.PID����/�ٶȿ���
extern struct PID Left_ctr;            //���ֿ������
extern struct PID Right_ctr;           //���ֿ������
extern struct PID_moshi ServoTurn;     //����������
//      5.Ԫ�ر�־
extern uint8 HuanDao_FLAG;
extern uint8 In_CheKu_flag;
//  ������
uint16 dir_smotor_mid;
uint16 dir_smotor_max = 780;
uint16 dir_smotor_min = 600;
float CDFTable[60];
//  ��ȡ����ƫ��
float mid_bias_add = 0;
float L_bias_add;
float R_bias_add;
float M_bias_add;
float L_Drop_k = 0.4;   //+=0.01
float R_Drop_k = 0.4;   //+=0.01
//  ת����ٶ�
float Track_R;              //��Сת��뾶
float TargetTurnAngSpeed;   //����ٶ�
//  ���Ʊ�־
int Min_SteerR = 20;
int16 Max_Car_Speed = 300;
/*--------------------------------------------------------------------------
* ���������ܡ��������ʼ��
* ����    ������
* ���� �� ֵ����
* ����    ע������
*--------------------------------------------------------------------------*/
void smotor_init()
{
    //�������   1.5ms/20ms * GTM_ATOM1_PWM_DUTY_MAX��GTM_ATOM1_PWM_DUTY_MAX��PWM����ռ�ձ�ʱ���ֵ�� GTM_ATOM1_PWM_DUTY_MAX��zf_gtm_pwm.h�ļ��� Ĭ��Ϊ10000
    dir_smotor_mid = dir_smotor_min + (dir_smotor_max - dir_smotor_min) / 2;
    gtm_pwm_init(S_MOTOR_PIN, 50, dir_smotor_max);  //��������ұ߽��Ƿ����
    systick_delay_ms(STM0, 500);
    gtm_pwm_init(S_MOTOR_PIN, 50, dir_smotor_min);      //�����������Ƿ����
    systick_delay_ms(STM0, 500);
    gtm_pwm_init(S_MOTOR_PIN, 50, dir_smotor_mid);      //�������
    systick_delay_ms(STM0, 500);
}
/*--------------------------------------------------------------------------
* ���������ܡ�������CDF��
* ����    ������
* ���� �� ֵ����
*--------------------------------------------------------------------------*/
void GetCDFTable()
{
#if 1 //0��30��
  for(int i = 0; i < 60; i++)
    CDFTable[i] = -0.00003 * i * i + 0.002 * i + 0.0785;
#endif

#if 0 //0��14��,b = 50
  for(int i = 0; i <= 30; i++)
    CDFTable[i] = -0.0002 * i * i + 0.0058 * i + 0.0744;
#endif

#if 0 //0��14��
  for(int i = 0; i <= 30; i++)
    CDFTable[i] = -0.00004 * i * i + 0.0011 * i + 0.0953;
#endif

#if 0 //0��11��
  for(int i = 0; i <= 30; i++)
    CDFTable[i] = -0.00004 * i * i + 0.0008 * i + 0.099;
#endif

#if 0
  for(int i = 0; i <= 30; i++)
    CDFTable[i] = -0.017 * i + 0.7549; //(15 - A:A)/22.5
#endif

#if 0
  for(int i = 0; i <= 30; i++)
    CDFTable[i] = 0.0000005 * i * i * i - 0.004 * i * i + 0.0315 * i + 0.9394; //����7.5
#endif

#if 0           //Ч������ 2m
  for(int i = 0; i <= 40; i++)
    CDFTable[i] = 0.00005* i * i * i - 0.0033 * i * i + 0.0214 * i + 0.9527; //����8
#endif

#if 0
  for(int i = 0; i <= 40; i++)
    CDFTable[i] = 0.00002* i * i * i - 0.0013 * i * i - 0.0047 * i + 0.9305; //����13
#endif

#if 0
  for(int i = 0; i <= 40; i++)
    CDFTable[i] = 0.00002* i * i * i - 0.0013 * i * i - 0.0013 * i + 0.9386; //����13 + 0.5
#endif

#if 0
  for(int i = 0; i <= 40; i++)
    CDFTable[i] = 0.00001* i * i * i - 0.001 * i * i + 0.0013 * i + 0.9524; //����15 + 0.5
#endif

#if 0
  for(int i = 0; i <= 40; i++)
    CDFTable[i] = 0.00001* i * i * i - 0.0014 * i * i + 0.0165 * i + 0.954; //(30 - A:A) / 10
#endif

#if 0           //��һ��
  for(int i = 0; i <= 40; i++)
    CDFTable[i] = 0.00001* i * i * i - 0.0014 * i * i + 0.0165 * i + 0.954; //(20 - A:A) / 8,Ȼ������˹�һ��
#endif

#if 0
  for(int i = 0; i <= 30; i++)
    CDFTable[i] = -2.0 / 900.0 * i + 2.0 / 30.0; //(20 - A:A) / 8,Ȼ������˹�һ��
#endif

}
/*--------------------------------------------------------------------------
* ���������ܡ�����ȡ����ƫ��
* ����    ������
* ���� �� ֵ����
*--------------------------------------------------------------------------*/
int Max_bias = 250;
int Min_bias = -250;
float testx;
void get_mid_bias()
{
    float k = 0.7;
    float index_add = 0.0;
    float Max = 0;
    float Min = 0;
    float Add = 0;
    static int index;
    static float bias_add_Filter[10];   //�˲���
    float bias_k[5] = {0.01647543, 0.06681116, 0.18161157, 0.330917856, 0.404183982};
    static float last_mid_bias_add;
    //��ʼ��
    L_bias_add = 0;
    R_bias_add = 0;
    M_bias_add = 0;
    last_mid_bias_add = mid_bias_add;
    //����ƫ��
    //  �������ߴ��ڣ����������ƫ��
    if(left_findflag)
    {
        for(int i=1;i<L_edge_count;i++)
        {
            if(L_edge[i].row>60)
                L_bias_add = L_bias_add + (float)((CDFTable[Image_H - L_edge[i].row]) * (1.0*(L_edge[i].col - L_edge[1].col) * 100.0/(L_edge[i].col + L_edge[1].col)));
        }
    }
    //  ����ұ��ߴ��ڣ������ұ���ƫ��
    if(right_findflag)
    {
        for(int i=1;i<R_edge_count;i++)
        {
            if(R_edge[i].row>60)
                R_bias_add = R_bias_add + (float)((CDFTable[Image_H - R_edge[i].row]) * ((R_edge[i].col - R_edge[1].col) * 100.0/(R_edge[i].col + R_edge[1].col)));
        }
    }
    //  ��������ƫ��
    for(int i = 1;i<Mid_count;i++)
    {
        if(Mid_Line[i].row>60)
            M_bias_add = M_bias_add + (float)((CDFTable[Image_H - Mid_Line[i].row]) * ((Mid_Line[i].col - Mid_Line[1].col) * 100.0/(Mid_Line[i].col + Mid_Line[1].col)));
    }
    testx = M_bias_add;
    mid_bias_add = 0;

    //  ����������ͼ�����ߵ�ƫ��
    for(int i = 1;i<Mid_count;i++)
    {
        if(Mid_Line[i].row>60)
            mid_bias_add = mid_bias_add + (float)((CDFTable[Image_H - Mid_Line[i].row]) * ((Mid_Line[i].col - Image_W/2) * 100.0/(Mid_Line[i].col + Image_W/2)));
    }
    mid_bias_add = mid_bias_add + (1-1.0*Mid_count/Image_H) * mid_bias_add;
    testx = mid_bias_add;
    //�Ż����߽�
    if(1)   //������ǻ��� HuanDao_FLAG==0
    {
        if(left_findflag&&!right_findflag)
            mid_bias_add = mid_bias_add + R_Drop_k * (L_bias_add + mid_bias_add)/2;
        else if(!left_findflag&&right_findflag)
            mid_bias_add = mid_bias_add + L_Drop_k * (R_bias_add + mid_bias_add)/2;
    }
    //�Ż�����ת��
    //�Ż�����ת��
    //�Ż��µ�ת��
    //�˲�
    for(int i = 0; i < 4; i++)
        bias_add_Filter[i] = bias_add_Filter[i + 1];
    bias_add_Filter[4] = mid_bias_add;
    mid_bias_add = 0;
    for(int i = 0; i < 5; i++)  //��˹�˲�
        mid_bias_add += (bias_k[i] * bias_add_Filter[i]);
    //�޷�
    mid_bias_add = (mid_bias_add>Max_bias ? Max_bias:mid_bias_add);
    mid_bias_add = (mid_bias_add<Min_bias ? Min_bias:mid_bias_add);

}
/*--------------------------------------------------------------------------
* ���������ܡ�����ȡ���������
* ����    ������
* ���� �� ֵ����
*--------------------------------------------------------------------------*/
float pure_bias;
void get_pure_bias()
{
    static float Ld = 0.0;              //Ԥ�����
    static float L = 20.0;              //�������
    static float dir_bias_angle = 0.0;  //�����Ƕ�
    static int row = 0;
    static int col = 0;

    pure_bias = 0;

    for(int i=1;i<=Mid_count;i++)
    {
        row = Image_H - Mid_Line[i].row;
        col = Mid_Line[i].col;

        if(col != Image_W/2)
        {
            Ld = 0.0027 * row * row * row -0.065 * row * row + 1.4081 * row +10 +22;
            dir_bias_angle = Get_angle(Image_W/2,0,Image_W/2,-5,col,row);
        }

        if(col>= Image_W/2)
            pure_bias = pure_bias + CDFTable[row] * 2 * L * mysin(dir_bias_angle)/Ld;
        else
            pure_bias = pure_bias - CDFTable[row] * 2 * L * mysin(dir_bias_angle)/Ld;
    }
}
/*--------------------------------------------------------------------------
* ���������ܡ�����ͼ������ƫ����ϰ뾶���õ�Ԥ��ת����ٶ�
* ����    ������
* ���� �� ֵ����
*--------------------------------------------------------------------------*/
void MidBias_FitTurnR()
{
    float bias = 0.0;
    int car_rel_speed = 0;

    if(0)//������ In_CheKu_flag!=0
        car_rel_speed = 400;
    else
        car_rel_speed = (Left_ctr.currspeed+Right_ctr.currspeed)/2;

//    if(In_CheKu_R_DaSi_flag == 1) //����Ҵ���
//        mid_bias_add = R_In_CK_bias;
    bias = mid_bias_add / 10.0;

    if(bias<0)
        Track_R = 1.0/(-0.00003*bias*bias-0.0016*bias);
    else if(bias>0)
        Track_R = -1.0/(-0.00003*bias*bias+0.0016*bias);
    else
        Track_R = 99999;

    TargetTurnAngSpeed = GetTargetTurnAngleSpeed(car_rel_speed,Track_R);
}
/*--------------------------------------------------------------------------
* ���������ܡ����ɳ��ٺ�ת��뾶����Ԥ��ת����ٶ�
* ����    ������
* ���� �� ֵ����
*--------------------------------------------------------------------------*/
float GetTargetTurnAngleSpeed(float car_speed, float steer_R)
{
    int   PMin_steerR = Min_SteerR;                                //��ת��Сת��뾶
    int   NMin_steerR = -Min_SteerR;                               //��ת��Сת��뾶
    float Max_carspeed = Max_Car_Speed;                             //����� 3.5m/s
    float Min_steerR = Min_SteerR;                                //��Сת��뾶
    float PMax_SteerAngleSpeed = Max_carspeed / Min_steerR;       //��ת���ת����ٶ�
    float NMax_SteerAngleSpeed = -PMax_SteerAngleSpeed;           //��ת���ת����ٶ�
    float steer_anglespeed = 0.0;                                 //ת����ٶ�
    //�޷�
    if(steer_R>0&&steer_R<PMin_steerR)
        steer_R = PMin_steerR;
    else if(steer_R<0&&steer_R>NMin_steerR)
        steer_R = NMin_steerR;
    steer_anglespeed = car_speed / steer_R;
    //�޷�
    steer_anglespeed = (steer_anglespeed > PMax_SteerAngleSpeed ? PMax_SteerAngleSpeed : steer_anglespeed);
    steer_anglespeed = (steer_anglespeed < NMax_SteerAngleSpeed ? NMax_SteerAngleSpeed : steer_anglespeed);

    return steer_anglespeed;
}
/*--------------------------------------------------------------------------
* ���������ܡ����������
* ����    ������ת���
* ���� �� ֵ����
*--------------------------------------------------------------------------*/
void ServoTurnControl()
{
    static float P_bias_k = 0.0;
    float D_bias_k = 0.0;
    static int L_col = 0;
    static int R_col = 0;
    float bias;
    float duty = 0.0;
    static float ServoOut_Filter[5] = { 0.0 };
    float ServoOut_k[5] = {0.01647543, 0.06681116, 0.18161157, 0.330917856, 0.404183982};
//    if(In_CheKu_flag!=0)
//    {
//        if ����Ҵ�����־ΪTRUE
//            mid_bias_add = ���̶����ֵ��
//        if ���ֱ�߱�־ΪTRUE
//            mid_biass_add = 0;
//    }
    bias = -mid_bias_add;
    if(bias<0)
        P_bias_k =  -0.000005 * bias * bias - 0.0029 * bias + 0.6;
    else
        P_bias_k = -0.000005 * bias * bias + 0.0029 * bias + 0.6;

    ServoTurn.error_last = ServoTurn.error_now;
    ServoTurn.error_now = bias;

    ServoTurn.kp_out = 1.0 * ServoTurn.kp * ServoTurn.error_now / 10.0;
    ServoTurn.kd_out = 1.0 * ServoTurn.kd * (ServoTurn.error_now - ServoTurn.error_last);

    ServoTurn.last_out = ServoTurn.output_val;
    ServoTurn.output_val = (ServoTurn.kp_out + ServoTurn.kd_out);

    Servo_Duty(ServoTurn.output_val);
}
#if 0
void smotor_control(uint8 col)
{
    uint16 s_dir;
    float k = 1.0 * (col - Image_W/2) / (Image_W*2 / 3);
    s_dir = dir_smotor_mid - (k+0.01*(col - Image_W/2)) * (dir_smotor_max - dir_smotor_mid);
    if(s_dir > 780) s_dir = 780;
    if(s_dir < 600) s_dir = 600;
    pwm_duty(S_MOTOR_PIN, s_dir);

}
#endif
/*--------------------------------------------------------------------------
* ���������ܡ���  ����ռ�ձȿ��ƶ��
* ����    ������     ռ�ձ�
* ���� �� ֵ����   ��
*--------------------------------------------------------------------------*/
extern int x1;
void Servo_Duty(int32 duty)
{
  int32 mid_duty = 690;
  int32 servo_duty = duty;
  int32 Max_duty = 780-10;    //��ֵ 70
  int32 Min_duty = 600+10;    //��ֵ -75 //��С��ƥ��

 // if(duty < 0)
 //   servo_duty = (int)(servo_duty * 70.0 / 75.0 + 0.5); //ѹ������

  servo_duty += mid_duty;
  x1 = duty;
  servo_duty = (servo_duty > Max_duty ? Max_duty : servo_duty);
  servo_duty = (servo_duty < Min_duty ? Min_duty : servo_duty);

  pwm_duty(S_MOTOR_PIN, servo_duty);
}
/*--------------------------------------------------------------------------
* ���������ܡ���  ��������ֵ
* ����    ������     �Ƕ�
* ���� �� ֵ����   ����ֵ
*--------------------------------------------------------------------------*/
extern float sin_value[91];
float mysin(float ang)
{
    float angle = 0.0;

    float delta = 0.0;

    float sin = 0.0;

    float k = 0.0;

    angle = (ang < 0 ? -ang : ang);

    delta = sin_value[(int)(ang + 1)] - sin_value[(int)ang];

    k = ang - (int)ang;

    sin = sin_value[(int)ang] + delta * k;

    if(ang >= 0)
      return sin;
    else
      return -sin;
}
