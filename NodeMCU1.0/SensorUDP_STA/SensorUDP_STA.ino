/*

发送端

ESP8266通过AP模式接收UDP信号
另一台通过STA模式发送UDP信号
实现P2P通讯


Wii手柄接线 (esp8266的I2C是可以定义引脚的，默认的是SDA为4，SCL为5号引脚)
Top Left 		SCK		Digital Pin 1 (GPIO05)
Top Right		GND		Ground Pin
Bottom L.		+ V		5 Volt Pin
Bottom R.		SDA		Digital Pin 2 (GOIO04)

*/

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

//Wii手柄依赖包
#include <Wire.h>
#include "Nunchuk.h"

const char *ssid = "M4";
const char *password = "ifconfig";

WiFiUDP Udp;
unsigned int port = 9999;
char packet[7];
IPAddress apip(192, 168, 4, 1);

int test_num = 0; //发包计数器

void setup()
{
    Serial.begin(115200);
    Serial.println();

    Serial.printf("Connecting to %s ", ssid);
    WiFi.mode(WIFI_STA);
    WiFi.setAutoConnect(false);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println(" connected");

    Wire.begin();
    nunchuk_init();

    //   Udp.begin(port);
    Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), port);
}

void loop()
{
    //   int packetSize = Udp.parsePacket();
    //   if (packetSize)
    //   {
    //     // receive incoming UDP packets
    //     Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    //     int len = Udp.read(incomingPacket, 255);
    //     if (len > 0)
    //     {
    //       incomingPacket[len] = 0;
    //     }
    //     Serial.printf("UDP packet contents: %s\n", incomingPacket);

    // send back a reply, to the IP address and port we got the packet from

    delay(100);
    if (nunchuk_read())
    {
        // Work with nunchuk_data
        nunchuk_print();
        packet[0] = nunchuk_joystickX()+100;
        packet[1] = nunchuk_joystickY();
        packet[2] = nunchuk_accelX();
        packet[3] = nunchuk_accelY();
        packet[4] = nunchuk_accelZ();
        packet[5] = nunchuk_buttonZ();
        packet[6] = nunchuk_buttonC();
        Udp.beginPacket(apip, port);
        Udp.write(packet);
        Udp.endPacket();
        // Serial.printf("UDP packet contents: %d  %d\n", packet[0], test_num);
        Serial.print(packet[0], DEC);
        Serial.println();
        // Serial.printf("UDP packet contents: %d,%d,%d,%d,%d,%d,%d  %d\n", packet[0],packet[1],packet[2],packet[3],packet[4],packet[5],packet[6],test_num);
        test_num++;
    }
    // delay(10);
    // return;

    //   }
}