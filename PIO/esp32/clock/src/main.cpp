#include <webservice.h>
void xTaskOne(void *xTask1)
{

    while(1) 
	{    

        Serial.printf("Task1 \r\n");
    }

}

void xTaskTwo(void *xTask2)
{
    while (1)
    {
        Serial.printf("Task2 \r\n");
        // Serial1.printf("Task2 \r\n");
        // delay(500);
    }
}
void setup() {
	webservice web;
    char ssid[]="Kalman",passphrase[]="abcdef..";
  	Serial.begin(115200); // open the serial port at 115200 bps;
	delay(100);
    // Serial1.begin(115200); // open the serial port at 115200 bps;
    // Serial1.print("ok");
	Serial.print("Connecting.. ");
	web.WiFi_Connect(ssid,passphrase);

  
	xTaskCreatePinnedToCore(xTaskOne, "TaskOne", 4096, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(xTaskTwo, "TaskTwo", 4096, NULL, 2, NULL, 1);
  // put your setup code here, to run once:
}

void loop() {
    Serial.printf("Task \r\n");
  // put your main code here, to run repeatedly:
}