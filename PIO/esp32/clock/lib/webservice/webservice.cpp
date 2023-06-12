#include "webservice.h"
void webservice :: WiFi_Connect(const char* ssid, const char *passphrase)
{
	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid, passphrase);
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
void webservice :: socket_connect(const char *host, uint16_t port)
{
  	WiFiClient client;
    while(!client.connect(host,port)) 
	{
    Serial.println("Connection failed.");
   Serial.println("Waiting 5 seconds before retrying...");
    delay(500);
	}
    Serial.println("Connection success.");
}