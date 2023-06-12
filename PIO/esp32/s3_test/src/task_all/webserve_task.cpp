#include "./task_all/task_all.h"
TaskHandle_t webserve_task;
QueueHandle_t webserve_QueueHandle;
void webserve_task_code(void * pvParameters)
{
  // WiFiServer server(8080);
  // const char* ssid = "Fourier";
  // const char* password =  "12345678";
  // const int port= 80;
  // WiFi.begin(ssid, password);




  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(1000);
  //   Serial.println("Connecting to WiFi...");
  // }
  // Serial.println("Connected to WiFi");
  // server.begin();
  // server.onClient([](WiFiClient client) {
  //   Serial.println("New client connected");
  //   while (client.connected()) {
  //     if (client.available()) {
  //       String request = client.readStringUntil('\r');
  //       Serial.println(request);
  //       client.print("Hello, client!");
  //     }
  //   }
  //   Serial.println("Client disconnected");
  // });


  for( ;; )
  {


    vTaskDelay(500);
  }
}