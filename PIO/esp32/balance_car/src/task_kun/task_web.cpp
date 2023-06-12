#include "task_web.h"
TaskHandle_t Task_web;
extern QueueHandle_t pid_queue;
// typedef struct {
// float P;
// float I;
// float D;
// } pid_dat;
void task_web(void *pvParameters)
{
    const char* ssid = "Fourier";
    const char* password = "12345678";
    const char* server = "192.168.31.145";
    const int port = 1234;

    pid_dat Pid;
    String line;
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
    vTaskDelay(100);
    Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");
    while (1)
    {
        WiFiClient client;
        while (!client.connected()) {
            if (client.connect(server, port)) {
            Serial.println("Connected to server");
            client.println("GET /data.json HTTP/1.1");
            client.println("Host: your_SERVER");
            client.println("Connection: close");
            client.println();
            } else {
            Serial.println("Failed to connect to server");
            vTaskDelay(1000);
            }
        }
        while (1)
        {
            while(!client.connected())
            {
                client.connect(server, port);
                vTaskDelay(500);
            }
            client.println("ok");
            while (client.connected()) {
                if (client.available()) {
                    line =client.readStringUntil('\n');
                    break;
                }
                vTaskDelay(5);
            }
            DynamicJsonDocument doc(1024);
            DeserializationError error = deserializeJson(doc,line);
            line=' ';
            if (error) {
                client.println("Failed to parse JSON");
            } else if(doc.containsKey("P")){
                client.println("JSON parsed successfully");
                Pid.P= doc["P"];
                Pid.I= doc["I"];
                Pid.D= doc["D"];
                xQueueOverwrite(pid_queue,&Pid);
                //  
            }
            // client.stop();
            vTaskDelay(5);
        }
        vTaskDelay(100);
    }
    
}
