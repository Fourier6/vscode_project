#ifndef webservice_h
#define webservice_h
#include <Arduino.h>
#include <WiFi.h>
class webservice
{
    public:
    void WiFi_Connect(const char* ssid, const char *passphrase);
    void socket_connect(const char *host, uint16_t port);
    private:
};
#endif