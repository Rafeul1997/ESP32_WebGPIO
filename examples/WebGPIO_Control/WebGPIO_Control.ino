#include <WebGPIO.h>

// 📶 WiFi credentials
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

// 🔌 GPIO pins
int myPins[] = {2, 4, 5, 18};

WebGPIO gpio(ssid, password);

void setup() {
  Serial.begin(115200);

  // 🌐 Optional: Set Static IP
  gpio.setStaticIP(
    IPAddress(192, 168, 1, 200),  // ESP32 IP
    IPAddress(192, 168, 1, 1),    // Gateway (router IP)
    IPAddress(255, 255, 255, 0)   // Subnet
  );

  // 🚀 Start system
  gpio.begin(myPins, 4);
}

void loop() {
  gpio.handleClient();
}
