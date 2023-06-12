
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define USE_MULTCORE 1
#define delay vTaskDelay
String ledst ="close";
void xTaskOne(void *xTask1)
{
	WiFiClient client;
	
    while(!client.connect("192.168.31.102", 6666)) 
	{
    Serial.println("Connection failed.");
   Serial.println("Waiting 5 seconds before retrying...");
    delay(500);
	}
    Serial.println("Connection success.");
    while (1)
    {
        // if (!client.available() )
		// {
		// 	Serial.print("no answer\r");
		// 	delay(1000); //延时 1 ms
		// }
  if (client.available() > 0)
  {
    	//读取服务器响应内容
    while(client.available()) {     //检测接收缓存区是否有内容
        String line = client.readStringUntil('\n');
        ledst = line;
        Serial1.print(line);
    }
  }
  
	// client.print("Send this data to the server");
    // Serial.println("send data success.\r\n");
    // Serial.printf("Task1 \r\n");
    if(ledst=="open")
    {
        digitalWrite(2, HIGH);
    }
    else
    {
        digitalWrite(2, LOW);
    }
	// delay(5000);
    }
}

void xTaskTwo(void *xTask2)
{
    while (1)
    {
        // Serial.printf("Task2 \r\n");
        // delay(1000);
    }
}

void WiFi_Connect()
{
	WiFi.mode(WIFI_STA);
	WiFi.begin("Kalman_5G", "abcdef..");
	while (WiFi.status() != WL_CONNECTED)
	{ //这里是阻塞程序，直到连接成功
		delay(300);
		Serial.print(".");
	}

  Serial.println("WiFi Connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP()); //打印模块IP
  Serial.print("subnetMask: ");
  Serial.println(WiFi.subnetMask()); //打印子网掩码
  Serial.print("gateway: ");
  Serial.println(WiFi.gatewayIP()); //打印网关地址
  Serial.print("dns: ");
  Serial.println(WiFi.dnsIP()); //打印DNS地址
  Serial.print("mac Address: ");
  Serial.println(WiFi.macAddress()); //打印mac地址
  Serial.print("HostName: ");
  Serial.println(WiFi.getHostname()); //打印主机名
  Serial.println(WiFi.status());

}

void setup()
{
    pinMode(2, OUTPUT);
	Serial.begin(115200); // open the serial port at 115200 bps;
	delay(100);
    Serial1.begin(115200,SERIAL_8N1,26,27); 
	Serial.print("Connecting.. ");
    Serial1.println("Serial1 success");
	WiFi_Connect();

	Serial.println("WiFi connected");
}
void loop()
{

#if !USE_MULTCORE

    xTaskCreate(
        xTaskOne,  /* Task function. */
        "TaskOne", /* String with name of task. */
        4096,      /* Stack size in bytes. */
        NULL,      /* Parameter passed as input of the task */
        1,         /* Priority of the task.(configMAX_PRIORITIES - 1 being the highest, and 0 being the lowest.) */
        NULL);     /* Task handle. */

    xTaskCreate(
        xTaskTwo,  /* Task function. */
        "TaskTwo", /* String with name of task. */
        4096,      /* Stack size in bytes. */
        NULL,      /* Parameter passed as input of the task */
        2,         /* Priority of the task.(configMAX_PRIORITIES - 1 being the highest, and 0 being the lowest.) */
        NULL);     /* Task handle. */

#else

    //最后一个参数至关重要，决定这个任务创建在哪个核上.PRO_CPU 为 0, APP_CPU 为 1,或者 tskNO_AFFINITY 允许任务在两者上运行.
    xTaskCreatePinnedToCore(xTaskOne, "TaskOne", 4096, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(xTaskTwo, "TaskTwo", 4096, NULL, 2, NULL, 1);

#endif

    while (1)
    {
 
        // Serial.printf("XTask is running\r\n");
        // delay(1000);
    }
}
