#include <Arduino.h>
#include <SimpleFOC.h>
#include <FreeRTOS.h>
#include <task.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include "U8g2lib.h"
#include <HardwareSerial.h>
Commander command = Commander(Serial);
// Motor 1
BLDCMotor motor1 = BLDCMotor(2);
BLDCDriver3PWM driver1 = BLDCDriver3PWM(35, 36, 37, 38);
// MagneticSensorSPI sensor1 = MagneticSensorSPI(10);

// Motor 2
BLDCMotor motor2 = BLDCMotor(2);
BLDCDriver3PWM driver2 = BLDCDriver3PWM(39, 40, 41, 42);
// MagneticSensorSPI sensor2 = MagneticSensorSPI(13);

// Task handles
TaskHandle_t task1Handle = NULL;
TaskHandle_t task2Handle = NULL;
TaskHandle_t task3Handle = NULL;
TaskHandle_t task4Handle = NULL;
TaskHandle_t task5Handle = NULL;

// Serial input buffer
char inputBuffer[32];
int inputIndex = 0;

float motor1_target,motor2_target;
// Task 1: Motor 1 control loop
// void xTarget(char* cmd) { command.scalar(&motor1.target, cmd); }
// void yTarget(char* cmd) { command.scalar(&motor2.target, cmd); }
void task1(void *pvParameters) {
  // Motor 1 setup
  driver1.voltage_power_supply = 12;
  driver1.init();
  // sensor1.init();
  motor1.linkDriver(&driver1);
  // motor1.linkSensor(&sensor1);
  motor1.voltage_limit = 4;
  motor1.velocity_limit = 1;
  motor1.controller = MotionControlType::angle_openloop;
  motor1.init();
  // command.add('X', xTarget, "x");
  while (1) {
    // motor1.loopFOC();
    motor1.move();
    vTaskDelay(1);
  }
}

// Task 2: Motor 2 control loop
void task2(void *pvParameters) {
  // Motor 2 setup
  driver2.voltage_power_supply = 12;
  driver2.init();
  // sensor2.init();
  motor2.linkDriver(&driver2);
  // motor2.linkSensor(&sensor2);
  motor2.voltage_limit = 4;
  motor2.velocity_limit = 1;
  motor2.controller = MotionControlType::angle_openloop;
  motor2.init();
  // command.add('Y', yTarget, "Y:");
  while (1) {
    // motor2.loopFOC();
    motor2.move();
    vTaskDelay(1);
  }
}
int distance=0;
int tx1_shape=1,tx1_w=-1,tx1_h=-1,tx1_r=-1,tx1_l1=-1,tx1_l2=-1,tx1_l3=-1,tx1_w_f=-1,tx1_h_f=-1,tx1_r_f=-1,tx1_l1_f=-1,tx1_l2_f=-1,tx1_l3_f=-1;

void task3(void *pvParameters) {
const char* ssid = "ABC";
const char* password = "";
const char* serverAddress = "192.168.1.101";
const int serverPort = 1234;
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.println("Connecting to WiFi...");
}
Serial.println("Connected to WiFi");
  while (1) {
    WiFiClient client;
    while (!client.connect(serverAddress, serverPort)) {
      Serial.println("Failed to connect to server");
      vTaskDelay(1000);
    }
    Serial.println("Connected to server");

    // Create JSON request
    DynamicJsonDocument doc(500);
    // doc["command"] = "set_target";
    // doc["target"] = 10.0;
    String request;
    // serializeJson(doc, request);

    // Send request to server
    client.print("Connected_ok");
    // Serial.println("Request sent:");
    // Serial.println(request);

    // Read response from server
    String response = "";
    while (client.connected())
    {
      response ="";
      while (client.connected()) {
        if (client.available()) {
          response =client.readStringUntil('\n');
          break;
        }
        // vTaskDelay(1);
      }
    DeserializationError error =deserializeJson(doc, response);
    if(error)
    {
    }else{
      if(doc.containsKey("shape"))
      {
        float x_temp=doc["X"];
        float y_temp=doc["Y"];
        motor1_target=motor1_target+0.1*x_temp;
        motor2_target=motor2_target+0.1*y_temp;
        Serial.printf("x:%.2f,x_t:%.2f\r\n",x_temp,motor1_target);
        if(motor1_target>3)
        {
          motor1_target=3;
        }
        if(motor1_target<-3)
        {
          motor1_target=-3;
        }
        if(motor2_target>3)
        {
          motor2_target=3;
        }
        if(motor2_target<-3)
        {
          motor2_target=-3;
        }
        motor1.target=motor1_target;
        motor2.target=motor2_target;
        tx1_shape=doc["shape"];
        switch (tx1_shape)
        {
        case 1:
          tx1_r_f=doc["R"];
          tx1_r=(float)distance*(float)tx1_r_f*0.003/3.2;
          break;
        case 2:
          tx1_w_f=doc["W"];
          tx1_h_f=doc["H"];
          tx1_w=(float)distance*(float)tx1_w_f*0.003/3.2;
          tx1_h=(float)distance*(float)tx1_h_f*0.003/3.2;
          break;
        case 3:
          tx1_l1_f=doc["L1"];
          tx1_l2_f=doc["L2"];
          tx1_l3_f=doc["L3"];
          tx1_l1=(float)distance*(float)tx1_l1_f*0.003/3.2;
          tx1_l2=(float)distance*(float)tx1_l2_f*0.003/3.2;
          tx1_l3=(float)distance*(float)tx1_l3_f*0.003/3.2;
        case 4:
          tx1_r_f=doc["R"];
          tx1_r=(float)distance*(float)tx1_r_f*0.003/3.2;
        case 5:
          tx1_r_f=doc["R"];
          tx1_r=(float)distance*(float)tx1_r_f*0.003/3.2;
        case 6:
          tx1_r_f=doc["R"];
          tx1_r=(float)distance*(float)tx1_r_f*0.003/3.2;
          break;
        default:
          break;
        }
      }
    }
    // vTaskDelay(1);
    }
    // Disconnect from server
    client.stop();
    Serial.println("Disconnected from server");
    // Wait for some time before reconnecting
    vTaskDelay(10);
  }
}

String inputString ="";
void IRAM_ATTR serial1_event(void)
{
while (Serial1.available() > 0) {
    char c = Serial1.read(); // 读取串口接收到的字符
    // Serial.println(c);
    if (c == '\n') { // 如果读取到了换行符
      Serial.print("data: ");
      Serial.println(inputString); // 输出接收到的数据
      distance=inputString.toInt()+18;

      inputString = ""; // 清空字符串变量，准备接收下一组数据
    } else {
      inputString += c; // 将字符添加到字符串变量中
    }
  }
}
void task4(void *pvParameters)
{
  U8G2_SSD1306_128X64_NONAME_1_SW_I2C u8g2(U8G2_R0, /* clock=*/ 2, /* data=*/ 1, /* reset=*/ U8X8_PIN_NONE);


  // Serial1.onReceive(serial1_event);

  u8g2.begin();
  u8g2.enableUTF8Print();
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x10_tf);

  while (1)
  {

    u8g2.firstPage();
    do {
      u8g2.setCursor(0, 7);
      u8g2.print("Distance=");
      u8g2.print(distance);
      u8g2.print("mm");
      u8g2.setCursor(0, 17);
      switch(tx1_shape)
      {
        case 2:u8g2.print("Shape=Rectangle");u8g2.setCursor(0, 27);u8g2.print("W=");u8g2.print(tx1_w);u8g2.print("mm");
        u8g2.setCursor(0, 37);u8g2.print("H=");u8g2.print(tx1_h);u8g2.print("mm");break;
        case 3:u8g2.print("shape=Triangle");u8g2.setCursor(0, 27);u8g2.print("L1=");u8g2.print(tx1_l1);u8g2.print("mm");
        u8g2.setCursor(0, 37);u8g2.print("L2=");u8g2.print(tx1_l2);u8g2.print("mm");
        u8g2.setCursor(0, 47);u8g2.print("L3=");u8g2.print(tx1_l3);u8g2.print("mm");break;
        case 1:u8g2.print("shape=Circular");u8g2.setCursor(0, 27);u8g2.print("R=");u8g2.print(tx1_r);u8g2.print("mm");break;
        case 4:u8g2.print("shape=Baskerball");u8g2.setCursor(0, 27);u8g2.print("R=");u8g2.print(tx1_r);u8g2.print("mm");break;
        case 5:u8g2.print("shape=Football");u8g2.setCursor(0, 27);u8g2.print("R=");u8g2.print(tx1_r);u8g2.print("mm");break;
        case 6:u8g2.print("shape=Volleyball");u8g2.setCursor(0, 27);u8g2.print("R=");u8g2.print(tx1_r);u8g2.print("mm");break;
        default:break;
      }
      // if()
    } while ( u8g2.nextPage()); 
    vTaskDelay(100);
  }
}

void task5(void *pvParameters)
{
  Serial1.begin(115200, SERIAL_8N1, 4, 5);
  while (1)
  {
    while (Serial1.available() > 0)
    {
      char c = Serial1.read(); // 读取串口接收到的字符
      // Serial.println(c);
      if (c == '\n') { // 如果读取到了换行符
        Serial.print("data: ");
        Serial.println(inputString); // 输出接收到的数据
        distance=inputString.toInt()+18;

        inputString = ""; // 清空字符串变量，准备接收下一组数据
      } else {
        inputString += c; // 将字符添加到字符串变量中
      }
    }
    vTaskDelay(100);
  }
  
}
const int touchPin = 5; // 电容触发引脚

int flag=1;
void IRAM_ATTR touchInterrupt() {
flag=0;// 熄灭LED灯
}
void setup() {
  Serial.begin(115200);
  touchAttachInterrupt(touchPin, touchInterrupt, 30000); // 绑定电容触发引脚和中断函数
  // Serial setup
  while(flag){
    Serial.println(touchRead(5));
    delay(300);
  }
  Serial.println(touchRead(5));
  touchDetachInterrupt(5);
  // Start tasks
  xTaskCreatePinnedToCore(task1, "Motor1", 5000, NULL, 1, &task1Handle, 0);
  xTaskCreatePinnedToCore(task2, "Motor2", 5000, NULL, 1, &task2Handle, 0);
  xTaskCreatePinnedToCore(task3, "Wifi", 30000, NULL, 1, &task3Handle, 1);
  xTaskCreatePinnedToCore(task4, "OLED", 5000, NULL, 1, &task4Handle, 1);
  xTaskCreatePinnedToCore(task5, "SERIAL1", 5000, NULL, 1, &task5Handle, 1);
}

void loop() {
  // command.run();
}
