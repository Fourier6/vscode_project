#include <Arduino.h>
int pwml,pwmr;
// the number of the LED pin
const int ledPin0 = 12;  // 15 corresponds to GPIO16
const int ledPin1 = 4;
// setting PWM properties
const int freq = 50;
const int ledChannel0= 0;
const int ledChannel1= 1;
const int resolution = 8;
const int AIN0 = 5;
const int AIN1 = 18;
const int AIN2 = 19;
const int AIN3 = 21;
void hx(char dat);
void PID(void);
char i;
// int interruptCounter = 0;
// hw_timer_t *timer = NULL;

// //	函数名称：onTimer()
// //	函数功能：中断服务的功能，它必须是一个返回void（空）且没有输入参数的函数
// //  为使编译器将代码分配到IRAM内，中断处理程序应该具有 IRAM_ATTR 属性
// //  https://docs.espressif.com/projects/esp-idf/zh_CN/release-v4.3/esp32/api-reference/storage/spi_flash_concurrency.html
// void IRAM_ATTR TimerEvent()
// {
//   if (Serial2.available() > 0){
//       char ch=char(Serial2.read());
//       // Serial2.println(ch);
//       if(ch>128)
//       {
//         pwmr=(int)((ch-128)*1.9);
//         pwml=0;
//       }    
//       else
//       {
//         pwml=(int)((128-ch)*1.9);
//         pwmr=0;
//       }
//   }

// } 
void setup(){
  Serial.begin(115200);
  Serial2.begin(115200);
  Serial.println("COM is ready");
  Serial2.println("Input Command :");
  // configure LED PWM functionalitites
  ledcSetup(ledChannel0, freq, resolution);
  ledcSetup(ledChannel1, freq, resolution);
  delay(100);
  ledcAttachPin(ledPin0, ledChannel0);
  ledcAttachPin(ledPin1, ledChannel1);
  // ledcWrite(ledChannel0, 24);
  // delay(1000);
  // ledcWrite(ledChannel0, 12);
  // delay(1000);
  Serial.println("ok");
  // attach the channel to the GPIO to be controlled
  // ledcAttachPin(ledPin0, ledChannel0);
  // ledcAttachPin(ledPin1, ledChannel1);
  //     //	函数名称：timerBegin()
  //   //	函数功能：Timer初始化，分别有三个参数
  //   //	函数输入：1. 定时器编号（0到3，对应全部4个硬件定时器）
  //   //			 2. 预分频器数值（ESP32计数器基频为80M，80分频单位是微秒）
  //   //			 3. 计数器向上（true）或向下（false）计数的标志
  //   //	函数返回：一个指向 hw_timer_t 结构类型的指针
  //   timer = timerBegin(0, 80, true);

  //   //	函数名称：timerAttachInterrupt()
  //   //	函数功能：绑定定时器的中断处理函数，分别有三个参数
  //   //	函数输入：1. 指向已初始化定时器的指针（本例子：timer）
  //   //			 2. 中断服务函数的函数指针
  //   //			 3. 表示中断触发类型是边沿（true）还是电平（false）的标志
  //   //	函数返回：无
  //   timerAttachInterrupt(timer, &TimerEvent, true);
     
  //   //	函数名称：timerAlarmWrite()
  //   //	函数功能：指定触发定时器中断的计数器值，分别有三个参数
  //   //	函数输入：1. 指向已初始化定时器的指针（本例子：timer）
  //   //			 2. 第二个参数是触发中断的计数器值（1000000 us -> 1s）
  //   //			 3. 定时器在产生中断时是否重新加载的标志
  //   //	函数返回：无
  //   timerAlarmWrite(timer, 20000, true);
  //   timerAlarmEnable(timer);
  pinMode(AIN0, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(AIN3, OUTPUT);
}
 
void loop(){
  ledcWrite(ledChannel0, 255-pwml);
  ledcWrite(ledChannel1, 255-pwmr);
  // while (1)
  // {
  //   ledcWrite(ledChannel0, 19);
  //   ledcWrite(ledChannel1, 255);
  // } 
  // i=i%4;
  // hx(i);
  // delay(5);
  // i++;

}
void hx(char dat)
{
  digitalWrite(AIN0, LOW);
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
  digitalWrite(AIN3, LOW);
  switch (dat)
  {
  case 0:digitalWrite(AIN0, HIGH);break;
  case 1:digitalWrite(AIN1, HIGH);break;
  case 2:digitalWrite(AIN2, HIGH);break;
  case 3:digitalWrite(AIN3, HIGH);break;
  
  default:
    break;
  }
}
void PID(void)
{
    if (Serial2.available() > 0){
      char ch=char(Serial2.read());
      // Serial2.println(ch);
      if(ch>128)
      {
        pwmr=(int)((ch-128)*1.9);
        pwml=0;
      }    
      else
      {
        pwml=(int)((128-ch)*1.9);
        pwmr=0;
      }
//   }
}