/*

发送端

ESP8266通过AP模式接收UDP信号
另一台通过STA模式发送UDP信号
实现P2P通讯
*/

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char *ssid = "M4";
const char *password = "ifconfig";

WiFiUDP Udp;
unsigned int port = 9999;
char  replyPacket[] = "Hi there! Got the message :-)";  // a reply string to send back
IPAddress ipwangg(192, 168, 4, 1);

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
    delay(1000);
    Udp.beginPacket(ipwangg, port);
    Udp.write(replyPacket);
    Udp.endPacket();
    Serial.printf("UDP packet contents: %s  %d\n", replyPacket,test_num);
    test_num++;
//   }
}