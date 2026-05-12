#include "WebGPIO.h"

WebGPIO* WebGPIO::instance = nullptr;

WebGPIO::WebGPIO(const char* ssid, const char* password)
  : _ssid(ssid), _password(password), server(80) {
  instance = this;
}

// 🌐 Set static IP
void WebGPIO::setStaticIP(IPAddress local_IP, IPAddress gateway, IPAddress subnet) {
  _local_IP = local_IP;
  _gateway = gateway;
  _subnet = subnet;
  _useStatic = true;
}

void WebGPIO::begin(int pins[], int count) {
  _pins = pins;
  _count = count;

  for (int i = 0; i < _count; i++) {
    pinMode(_pins[i], OUTPUT);
    digitalWrite(_pins[i], LOW);
    _state[i] = false;
  }

  // 🌐 Apply static IP if enabled
  if (_useStatic) {
    if (!WiFi.config(_local_IP, _gateway, _subnet)) {
      Serial.println("Static IP Failed!");
    }
  }

  WiFi.begin(_ssid, _password);

  Serial.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", []() { instance->handleRoot(); });
  server.on("/toggle", []() { instance->handleToggle(); });

  server.begin();
}

void WebGPIO::handleClient() {
  server.handleClient();
}

String WebGPIO::generateHTML() {
  String html = "<!DOCTYPE html><html><head>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<style>button{padding:15px;margin:10px;font-size:18px}</style>";
  html += "</head><body><h2>ESP32 GPIO Control</h2>";

  for (int i = 0; i < _count; i++) {
    html += "<p>GPIO ";
    html += _pins[i];
    html += " - ";
    html += (_state[i] ? "ON" : "OFF");
    html += "</p>";

    html += "<a href='/toggle?pin=";
    html += i;
    html += "'><button>Toggle GPIO ";
    html += _pins[i];
    html += "</button></a>";
  }

  html += "</body></html>";
  return html;
}

void WebGPIO::handleRoot() {
  server.send(200, "text/html", generateHTML());
}

void WebGPIO::handleToggle() {
  if (server.hasArg("pin")) {
    int index = server.arg("pin").toInt();

    if (index >= 0 && index < _count) {
      _state[index] = !_state[index];
      digitalWrite(_pins[index], _state[index]);
    }
  }

  server.sendHeader("Location", "/");
  server.send(303);
}
