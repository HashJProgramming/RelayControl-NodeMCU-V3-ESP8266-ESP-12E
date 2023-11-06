#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#define WIFI_SSID ""
#define WIFI_PASS ""

const int RELAY1_PIN = D1;
const int RELAY2_PIN = D2;
ESP8266WebServer server(80);
MDNSResponder MDNS;

bool BULBstatus1 = HIGH;
bool BULBstatus2 = HIGH;

void handleRoot()
{
  String output = "<html><head><title>SmartLight Control</title>";
  output += "<style>html {font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}";
  output += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}";
  output += ".button {margin-top: 30px; display: block;background-color: #1abc9c;border: none;color: white;padding: 2em 3em;text-decoration: none;font-size: 5em;margin: 0px auto 35px;cursor: pointer;border-radius: 10px;}";
  output += ".button-on {background-color: #3498db;}";
  output += ".button-on:active {background-color: #3498db;}";
  output += ".button-off {background-color: #34495e;}";
  output += ".button-off:active {background-color: #2c3e50;}";
  output += "p {font-size: 14px;color: #888;margin-bottom: 10px;}";
  output += "</style>";
  output += "</head><body>";
  output += "<h1 style='font-size: 4em;'>SmartLight Control</h1>";
  output += "<h1 style='font-size: 3em;'>Light Bulb 1</h1>";
  if (BULBstatus1 == LOW)
  {
    output += "<a href='relay1/off' style='text-decoration:none'><button class='button button-off'>Turn Off</button></a>";
  }
  else if (BULBstatus1 == HIGH)
  {
    output += "<a href='relay1/on' style='text-decoration:none'><button class='button button-on'>Turn On</button></a>";
  }
  output += "<h1 style='font-size: 3em;'>Light Bulb 2</h1>";
  if (BULBstatus2 == LOW)
  {
    output += "<a href='relay2/off' style='text-decoration:none'><button class='button button-off'>Turn Off</button></a>";
  }
  else if (BULBstatus2 == HIGH)
  {
    output += "<a href='relay2/on' style='text-decoration:none'><button class='button button-on'>Turn On</button></a>";
  }
  output += "</body></html>";
  server.send(200, "text/html", output);
}

void handleRelay1On()
{
  digitalWrite(RELAY1_PIN, LOW);
  delay(100);
  BULBstatus1 = LOW;
  digitalWrite(LED_BUILTIN, LOW);
  server.sendHeader("Location", String("/"), true);
  server.send(302, "text/plain", "");
}

void handleRelay1Off()
{
  digitalWrite(RELAY1_PIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, HIGH);
  BULBstatus1 = HIGH;
  server.sendHeader("Location", String("/"), true);
  server.send(302, "text/plain", "");
}

void handleRelay2On()
{
  digitalWrite(RELAY2_PIN, LOW);
  delay(100);
  BULBstatus2 = LOW;
  server.sendHeader("Location", String("/"), true);
  server.send(302, "text/plain", "");
}

void handleRelay2Off()
{
  digitalWrite(RELAY2_PIN, HIGH);
  delay(100);
  BULBstatus2 = HIGH;
  server.sendHeader("Location", String("/"), true);
  server.send(302, "text/plain", "");
}

void status()
{
  server.send(200, "text/plain", "Bulb 1 status: " + String(BULBstatus1) + "\nBulb 2 status: " + String(BULBstatus2));
}
void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(RELAY1_PIN, OUTPUT);
  pinMode(RELAY2_PIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(RELAY1_PIN, HIGH);
  digitalWrite(RELAY2_PIN, HIGH);
  WiFi.softAP("SmartLight", "12345678");
  WiFi.setHostname("SmartLight");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  if (MDNS.begin("SmartLight", WiFi.localIP()))
  {
    server.on("/", handleRoot);
    server.on("/relay1/on", HTTP_GET, handleRelay1On);
    server.on("/relay1/off", HTTP_GET, handleRelay1Off);
    server.on("/relay2/on", HTTP_GET, handleRelay2On);
    server.on("/relay2/off", HTTP_GET, handleRelay2Off);
    server.on("/status", HTTP_GET, status);
    server.begin();
    MDNS.addService("http", "tcp", 80);
  }
}

void loop()
{
  server.handleClient();
}
