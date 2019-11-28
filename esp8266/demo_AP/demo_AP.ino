#include <ESP8266WiFi.h>

extern "C"
{
#include <user_interface.h>
}

void setup()
{
    Serial.begin(115200);
    Serial.println();

    uint8_t macAddr[6]{0x88, 0xd7, 0x63, 0x00, 0x0e, 0x02};
    wifi_set_macaddr(SOFTAP_IF, macAddr);

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