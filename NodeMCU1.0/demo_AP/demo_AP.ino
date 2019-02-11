#include <ESP8266WiFi.h>

void setup()
{
    Serial.begin(115200);
    Serial.println();

    Serial.print("Setting soft-AP ... ");
    Serial.println(WiFi.softAP("M4", "ifconfig") ? "Ready" : "Failed!");

    Serial.println();
    Serial.print("Server IP address: ");
    Serial.println(WiFi.softAPIP());
    Serial.print("Server MAC address: ");
    Serial.println(WiFi.softAPmacAddress());
}

void loop()
{
    Serial.printf("Stations connected = %d\n", WiFi.softAPgetStationNum());
    delay(3000);
}