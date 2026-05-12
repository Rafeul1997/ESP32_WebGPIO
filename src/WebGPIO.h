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

  private:
    const char* _ssid;
    const char* _password;

    WebServer server;

    int* _pins;
    int _count;
    bool _state[20];

    String generateHTML();
    void handleRoot();
    void handleToggle();

    static WebGPIO* instance;
};

#endif
