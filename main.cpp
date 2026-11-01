
#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

// WiFi Details
const char* ssid = "xxxxxxxx";
const char* password = "xxxxxxxxx";

// Telegram Details
#define BOT_TOKEN "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
#define CHAT_ID "1076704461"

WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);

WebServer server(80);

// GPIO Pins
#define SENSOR_PIN 27
#define RED_LED 25
#define YELLOW_LED 32
#define GREEN_LED 33
#define RELAY_PIN 26

bool systemArmed = false;
bool alertActive = false;

int intrusionCount = 0;
int lastSensorState = HIGH;

unsigned long lastDetectionTime = 0;
const unsigned long detectionDelay = 3000;

// Relay is assumed active LOW
void buzzerOn() {
  digitalWrite(RELAY_PIN, LOW);
}

void buzzerOff() {
  digitalWrite(RELAY_PIN, HIGH);
}

void updateLEDs() {
  digitalWrite(GREEN_LED, systemArmed ? HIGH : LOW);

  digitalWrite(YELLOW_LED, systemArmed ? LOW : HIGH);

  digitalWrite(RED_LED, alertActive ? HIGH : LOW);
}

void sendTelegramMessage(String message) {
  if (WiFi.status() == WL_CONNECTED) {
    bot.sendMessage(CHAT_ID, message, "");
  }
}

// Dashboard
void handleRoot() {
  String page = R"rawliteral(
  <!DOCTYPE html>
  <html>
  <head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>ESP32 SECURITY SYSTEM</title>
  <style>
  body {
    font-family: Arial;
    text-align: center;
    background: #172033;
    color: white;
  }
  h1 { color: #00d9ff; }
  button {
    padding: 15px 30px;
    margin: 10px;
    font-size: 18px;
    border: none;
    border-radius: 8px;
  }
  .arm { background: #16a34a; color: white; }
  .disarm { background: #dc2626; color: white; }
  .card {
    background: #263449;
    padding: 20px;
    margin: 20px;
    border-radius: 12px;
  }
  </style>
  </head>
  <body>

  <h1>ESP32 SECURITY SYSTEM</h1>

  <div class="card">
  <h2>System Status: <span id="status">Loading</span></h2>
  <h2>Intrusion Count: <span id="count">0</span></h2>
  <h2>Alert: <span id="alert">OFF</span></h2>
  </div>

  <button class="arm" onclick="fetch('/arm')">ARM</button>
  <button class="disarm" onclick="fetch('/disarm')">DISARM</button>

  <script>
  async function updateStatus() {
    let response = await fetch('/status');
    let data = await response.json();

    document.getElementById('status').innerText =
      data.armed ? 'ARMED' : 'DISARMED';

    document.getElementById('count').innerText =
      data.count;

    document.getElementById('alert').innerText =
      data.alert ? 'INTRUSION DETECTED' : 'OFF';
  }

  setInterval(updateStatus, 1000);
  updateStatus();
  </script>

  </body>
  </html>
  )rawliteral";

  server.send(200, "text/html", page);
}

void handleArm() {
  systemArmed = true;
  alertActive = false;
  buzzerOff();
  updateLEDs();

  server.send(200, "text/plain", "SYSTEM ARMED");
}

void handleDisarm() {
  systemArmed = false;
  alertActive = false;
  buzzerOff();
  updateLEDs();

  server.send(200, "text/plain", "SYSTEM DISARMED");
}

void handleStatus() {
  String json = "{";
  json += "\"armed\":";
  json += systemArmed ? "true" : "false";
  json += ",\"count\":";
  json += String(intrusionCount);
  json += ",\"alert\":";
  json += alertActive ? "true" : "false";
  json += "}";

  server.send(200, "application/json", json);
}

void setup() {
  Serial.begin(115200);

  pinMode(SENSOR_PIN, INPUT);

  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);

  buzzerOff();
  updateLEDs();

  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  secured_client.setInsecure();

  server.on("/", handleRoot);
  server.on("/arm", handleArm);
  server.on("/disarm", handleDisarm);
  server.on("/status", handleStatus);

  server.begin();

  Serial.println("Web Server Started");
}

void loop() {
  server.handleClient();

  int sensorState = digitalRead(SENSOR_PIN);

  if (systemArmed &&
      lastSensorState == HIGH &&
      sensorState == LOW &&
      millis() - lastDetectionTime > detectionDelay) {

    intrusionCount++;
    alertActive = true;

    updateLEDs();
    buzzerOn();

    Serial.println("INTRUSION DETECTED!");

    sendTelegramMessage(
      "ALERT! Intrusion Detected!"
    );

    lastDetectionTime = millis();
  }

  lastSensorState = sensorState;

  delay(20);
}