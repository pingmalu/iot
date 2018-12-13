/*
  To upload through terminal you can use: curl -u admin:admin -F "image=@firmware.bin" esp8266-webupdate.local/firmware
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>

#ifndef STASSID
#define STASSID "ssid"
#define STAPSK  "passwd"
#endif

const char* host = "192.168.11.246";
const char* update_path = "/update";
const char* update_username = "malu";
const char* update_password = "malu";
const char* ssid = STASSID;
const char* password = STAPSK;

// IPAddress ip(192, 168, 11, 246);
// IPAddress gateway(192, 168, 11, 2);
// IPAddress subnet(255, 255, 255, 0);
// IPAddress dns(221,6,4,66);

ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;

void setup(void) {

  Serial.begin(9600);
  Serial.println();
  Serial.println("Booting Sketch...");
  WiFi.mode(WIFI_AP_STA);
//   WiFi.config(ip, gateway, subnet, dns);
  WiFi.begin(ssid, password);

  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    // delay(500);
    // WiFi.config(ip, gateway, subnet, dns);
    WiFi.begin(ssid, password);
    Serial.println("WiFi failed, retrying.");
  }

  MDNS.begin(host);

  httpUpdater.setup(&httpServer, update_path, update_username, update_password);
  httpServer.begin();

  MDNS.addService("http", "tcp", 80);
  Serial.printf("HTTPUpdateServer ready! Open http://%s.local%s in your browser and login with username '%s' and password '%s'\n", host, update_path, update_username, update_password);
}

void loop(void) {
  httpServer.handleClient();
}
