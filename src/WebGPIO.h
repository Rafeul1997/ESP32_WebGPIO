#ifndef WEBGPIO_H
#define WEBGPIO_H

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

class WebGPIO {
  public:
    WebGPIO(const char* ssid, const char* password);

    void begin(int pins[], int count);
    void handleClient();

    // 🌐 Static IP config (optional)
    void setStaticIP(IPAddress local_IP, IPAddress gateway, IPAddress subnet);

  private:
    const char* _ssid;
    const char* _password;

    WebServer server;

    int* _pins;
    int _count;
    bool _state[20];

    // Static IP storage
    IPAddress _local_IP;
    IPAddress _gateway;
    IPAddress _subnet;
    bool _useStatic = false;

    String generateHTML();
    void handleRoot();
    void handleToggle();

    static WebGPIO* instance;
};

#endif
