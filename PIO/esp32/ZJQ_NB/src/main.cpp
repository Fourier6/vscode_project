#include <Arduino.h>
uint8_t AIN1=5,AIN2=18,AIN3=19,AIN4=21,PWM1=0,PWM2=1,i=0,zx=80,PWML=100,PWMR=100;
int L,R;
int D=0;
void setup(){
  Serial.begin(115200);
  Serial2.begin(115200);
  pinMode(AIN2, OUTPUT);
  pinMode(AIN4, OUTPUT);
  digitalWrite(AIN2, LOW);
  digitalWrite(AIN4, LOW);
  ledcSetup(PWM1, 1000, 8);
  ledcAttachPin(AIN1, PWM1);
  ledcWrite(PWM1,0);
  ledcSetup(PWM2, 1000, 8);
  ledcAttachPin(AIN3, PWM2);
  ledcWrite(PWM2,0);
}
void loop() {
  while(1){
  if(Serial2.available())
  {
    zx=Serial2.read();
    // D=(zx-80)*0.1+D;
    // if(D>150)
    // {
    //   D=150;
    // }else if(D<-150)
    // {
    //   D=-150;
    // }
    // if((D>0&&zx<80)||(D<0&&zx>80))
    // {
    //   D=0;
    // }

    // while(Serial2.read());
    if(zx>80)
    {
      L=PWML+(((zx-80))*1.2)+D;
    // if(L>250)
    // {
    //   L=250;
    // }else if(L<PWML)
    // {
    //   L=PWML;
    // }
      ledcWrite(PWM1,(uint8_t)L);
      ledcWrite(PWM2,PWMR-0.3*L);
    }else{
      R=PWMR+((80-zx)*1.2)-D;
    // if(R>250)
    // {
    //   R=250;
    // }else if(R<PWMR)
    // {
    //   R=PWMR;
    // }
      ledcWrite(PWM1,PWML-0.3*R);
      ledcWrite(PWM2,(uint8_t)R);      
    }
     Serial.println(R);
  }
  }
}