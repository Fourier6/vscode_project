#include "./task_all/task_all.h"
TaskHandle_t webserve_task;
QueueHandle_t webserve_QueueHandle;
void webserve_task_code(void * pvParameters)
{
  // WiFiServer server(8080);
  const char* ssid = "Fourier";
  const char* password =  "12345678";
  const int port= 80;
  WiFi.begin(ssid, password);

  AsyncWebServer server(port);
  while (WiFi.status() != WL_CONNECTED)
  {
    vTaskDelay(300);
    
		Serial_println("Connecting to WiFi...");
  }
		Serial_println("wifi_ok");

  server.on("/websocket", HTTP_GET, [](AsyncWebServerRequest *request){
      if (request->hasParam("name") && request->hasParam("age")) {
        String name = request->getParam("name")->value();
        int age = request->getParam("age")->value().toInt();
        // Serial_println("Name: " + name);
        // Serial_println("Age: " + String(age));
        DynamicJsonDocument doc(1024);
        doc["status"] = "success";
        String response;
        serializeJson(doc, response);
        request->send(200, "application/json",response.c_str());
      } else {
        request->send(400, "text/plain", "Bad Request");
      }
    });
    server.begin();
  

  while (1)
  {
    // server.on("/websocket", HTTP_GET, [](AsyncWebServerRequest *request){
    //   if (request->hasParam("name") && request->hasParam("age")) {
    //     String name = request->getParam("name")->value();
    //     int age = request->getParam("age")->value().toInt();
    //     Serial_println(("Name: " + name).c_str());
    //     Serial_println(("Age: " + String(age)).c_str());
    //     DynamicJsonDocument doc(1024);
    //     doc["status"] = "success";
    //     String response;
    //     serializeJson(doc, response);
    //     request->send(200, "application/json",response.c_str());
    //   } else {
    //     request->send(400, "text/plain", "Bad Request");
    //   }
    // });
    // server.begin();

    vTaskDelay(500);
  }
}