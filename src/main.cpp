
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#define  WIFI_SSID ""
#define  WIFI_PASS ""

const int RELAY1_PIN = D1;
const int RELAY2_PIN = D2;
ESP8266WebServer server(80);

void handleRoot() {
  String output = "<html><head><title>Hash'J HouseLights</title>";
  output += "<link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css\">";
  output += "<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js\"></script>";
  output += "<script src=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js\"></script>";
  output += "</head><body>";
  output += "<div class=\"container\">";
  output += "<h1>Hash'J HouseLights</h1>";
  output += "<div class=\"row\">";
  output += "<div class=\"col-md-6\">";
  output += "<h2>Relay 1</h2>";
  output += "<button id=\"relay1-on\" class=\"btn btn-success\">Turn On</button>";
  output += "<button id=\"relay1-off\" class=\"btn btn-danger\">Turn Off</button>";
  output += "</div>";
  output += "<div class=\"col-md-6\">";
  output += "<h2>Relay 2</h2>";
  output += "<button id=\"relay2-on\" class=\"btn btn-success\">Turn On</button>";
  output += "<button id=\"relay2-off\" class=\"btn btn-danger\">Turn Off</button>";
  output += "</div>";
  output += "</div>";
  output += "</div>";
  output += "<script>";
  output += "$('#relay1-on').click(function() {";
  output += "$.get('/relay1/on');";
  output += "});";
  output += "$('#relay1-off').click(function() {";
  output += "$.get('/relay1/off');";
  output += "});";
  output += "$('#relay2-on').click(function() {";
  output += "$.get('/relay2/on');";
  output += "});";
  output += "$('#relay2-off').click(function() {";
  output += "$.get('/relay2/off');";
  output += "});";
  output += "</script>";
  output += "</body></html>";
  server.send(200, "text/html", output);
}

void handleRelay1On() {
  digitalWrite(RELAY1_PIN, LOW);
  server.send(200, "text/plain", "Relay 1 turned on");
}

void handleRelay1Off() {
  digitalWrite(RELAY1_PIN, HIGH);
  server.send(200, "text/plain", "Relay 1 turned off");
}

void handleRelay2On() {
  digitalWrite(RELAY2_PIN, LOW);
  server.send(200, "text/plain", "Relay 2 turned on");
}

void handleRelay2Off() {
  digitalWrite(RELAY2_PIN, HIGH);
  server.send(200, "text/plain", "Relay 2 turned off");
}

void setup() {
  pinMode(RELAY1_PIN, OUTPUT);
  digitalWrite(RELAY1_PIN, HIGH);
  pinMode(RELAY2_PIN, OUTPUT);
  digitalWrite(RELAY2_PIN, HIGH);
  WiFi.setHostname("houselights");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  MDNS.begin("houselights");
  Serial.println("MDNS responder started");
  server.on("/", handleRoot);
  server.on("/relay1/on", handleRelay1On);
  server.on("/relay1/off", handleRelay1Off);
  server.on("/relay2/on", handleRelay2On);
  server.on("/relay2/off", handleRelay2Off);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
