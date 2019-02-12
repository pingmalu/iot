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

nunchuk_accelX()  最大值 506

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
char packet[6];
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

    delay(10);
    if (nunchuk_read())
    {
        // Work with nunchuk_data
        packet[0] = nunchuk_joystickX()+123;
        packet[1] = nunchuk_joystickY()+126;
        // packet[2] = nunchuk_accelX()+360;
        // packet[3] = nunchuk_accelY()+360;
        // packet[4] = nunchuk_accelZ()+360;
        packet[2] = nunchuk_buttonZ()+1;
        packet[3] = nunchuk_buttonC()+1;
        packet[4] = map(constrain(nunchuk_accelX(),-260,100),-260,100,1,255);
        packet[5] = map(constrain(nunchuk_accelY(),-200,160),-200,160,1,255);
        packet[6] = map(constrain(nunchuk_accelZ(),-200,160),-200,160,1,255);

        Udp.beginPacket(apip, port);
        Udp.write(packet);
        Udp.endPacket();
        // Serial.printf("UDP packet contents: %d  %d\n", packet[0], test_num);
        Serial.print(packet[0], DEC);
        Serial.print(",");
        Serial.print(packet[1], DEC);
        Serial.print(",");
        Serial.print(packet[2], DEC);
        Serial.print(",");
        Serial.print(packet[3], DEC);
        Serial.print(",");
        Serial.print(packet[4], DEC);
        Serial.print(",");
        Serial.print(packet[5], DEC);
        Serial.print(",");
        Serial.print(packet[6], DEC);
        Serial.print(" ");
        // memcpy(&BTValue, &packet[4], 2);
        // if(BTValue<maxBTValue){
        //     maxBTValue=BTValue;
        // }
        // -200 160
        // Serial.print(constrain(maxBTValue,-260,100));
        // Serial.print("-");
        // Serial.print(map(constrain(maxBTValue,-260,100),-260,100,1,255));
        // Serial.print(",");
        // memcpy(&BTValue, &packet[6], 2);
        // Serial.print(BTValue, DEC);
        // Serial.print(",");
        // memcpy(&BTValue, &packet[8], 2);
        // Serial.print(BTValue, DEC);

        Serial.printf(" test_num: %d ", test_num);
        nunchuk_print();
        // // Serial.println();
        // // Serial.printf("UDP packet contents: %d,%d,%d,%d,%d,%d,%d  %d\n", packet[0],packet[1],packet[2],packet[3],packet[4],packet[5],packet[6],test_num);
        test_num++;
    }
    // delay(10);
    // return;

    //   }
}