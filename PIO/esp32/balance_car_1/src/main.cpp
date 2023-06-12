#include <Arduino.h>
// #include "../lib/motor_bl/motor_bl.h"
// #include "../lib/speed_bl/speed_bl.h"
#define ADC_RESOLUTION 4095
#define ADC_REF_VOLTAGE 3.3
int nun;
uint8_t piexl[128];
uint8_t ADC_PIN=4,TSL_CLK=18,TSL_SI=19,ccd_adc[128];
void RD_TSL(void) ;
void setup() {
  pinMode(TSL_CLK, OUTPUT);
  pinMode(TSL_SI, OUTPUT);
  // 配置ADC通道
  // analogSetPinAttenuation(ADC_PIN, ADC_ATTEN_DB_11);
  // 设置串口波特率
  Serial.begin(115200);
}

void loop() {
  // // 读取ADC值
  // int adcValue = analogRead(ADC_PIN);
  // // 将ADC值转换为电压值
  // float voltage = adcValue * ADC_REF_VOLTAGE / ADC_RESOLUTION;
  // // 输出结果
  // Serial.print("ADC value: ");
  // Serial.print(adcValue);
  // Serial.print(", voltage: ");
  // Serial.println(voltage);
  // // 延时一段时间
  unsigned long first=micros();
  RD_TSL();
  unsigned long last=micros();
  unsigned long time=last-first;

     Serial.write((byte)0x01);
     Serial.write((byte)0x00);
  for (int i = 0; i < sizeof(piexl) / sizeof(uint8_t); i++) {
  // printf("%d,", piexl[i]);
  if(piexl[i]>=0xfe)
  {
    piexl[i]=0xfe;
  }else if(piexl[i]<0x03)
  {
    piexl[i]=0x03;
  }
  Serial.write(piexl[i]);
  }
       Serial.write((byte)0x00);
     Serial.write((byte)0xFF);
  
  // Serial.printf("\r\n");
  

} 
#define CCD_CLK TSL_CLK
#define CCD_SI TSL_SI

uint32_t TIME_us=1;
void RD_TSL(void) 
{
    int exp_time = 10000;
    int i = 0;
     digitalWrite(CCD_SI,HIGH);                         //SI拉高电平
     digitalWrite(CCD_CLK,HIGH);                        //时钟高电平
     digitalWrite(CCD_SI,LOW);                          //SI低电平
     digitalWrite(CCD_CLK,LOW);                         //时钟低电平   
    for(i=0;i<128;i++)
     {
         digitalWrite(CCD_CLK,HIGH);
         digitalWrite(CCD_CLK,LOW);
     }                                              //从这里结束曝光
     delayMicroseconds(exp_time);                       //曝光时间
     digitalWrite(CCD_SI,HIGH);
     digitalWrite(CCD_CLK,HIGH);
     digitalWrite(CCD_SI,LOW);
     digitalWrite(CCD_CLK,LOW);
    for(i=0;i<128;i++)
    {
        digitalWrite(CCD_CLK,HIGH);
        piexl[i]=(float)analogRead(ADC_PIN)/4096*255;      
        digitalWrite(CCD_CLK,LOW);
    }
    digitalWrite(CCD_CLK,HIGH);
    digitalWrite(CCD_CLK,LOW);
}
