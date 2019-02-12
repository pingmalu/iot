/*

接收端

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
char packet[9];

int test_num = 0; //发包计数器

int BTValue;

void setup()
{
    Serial.begin(115200);
    Serial.println();

    Serial.print("Setting soft-AP ... ");
    Serial.println(WiFi.softAP(ssid, password) ? "Ready" : "Failed!");

    Serial.println();
    Serial.print("Server IP address: ");
    Serial.println(WiFi.softAPIP());
    Serial.print("Server MAC address: ");
    Serial.println(WiFi.softAPmacAddress());

    Udp.begin(port);
}

void loop()
{
    // Serial.printf("Stations connected = %d\n", WiFi.softAPgetStationNum());
    // delay(3000);

    int packetSize = Udp.parsePacket();
    if (packetSize)
    {
        int len = Udp.read(packet, 9);
        Serial.printf("Received %d bytes from %s, port %d, len %d, test_num %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort(), len, test_num);

        if (len > 0)
        {
            // packet[len] = 0; //末尾补0结束字符串
            // Serial.printf("UDP packet contents: %s\n", packet);
            // Serial.println(packet[0]);
            // Serial.printf("UDP packet contents: %f,%f,%d,%d,%d,%d,%d  %d\n", ntohf(packet[0]),ntohf(packet[1]),packet[2],packet[3],packet[4],packet[5],packet[6],test_num);

            Serial.print((int)packet[0]);
            Serial.print(",");
            Serial.print((int)packet[1]);
            Serial.print(",");
            Serial.print((int)packet[2]);
            Serial.print(",");
            Serial.print((int)packet[3]);
            Serial.print(",");
            memcpy(&BTValue, &packet[4], 2);
            Serial.print(BTValue);
            Serial.print(",");
            memcpy(&BTValue, &packet[6], 2);
            Serial.print(BTValue);
            Serial.print(",");
            memcpy(&BTValue, &packet[8], 2);
            Serial.print(BTValue, DEC);
            Serial.println();
            test_num++;

            // int32_t bigEndianValue;

            // Serial.print(' ');
            // memcpy(&bigEndianValue, &packet[0], 4);
            // Serial.println(ntohf(bigEndianValue));
            // Serial.println(packet);

            // memcpy(&bigEndianValue, &packet[0], 4);
            // float theUnpackedValue = ntohf(bigEndianValue);
            // Serial.print(theUnpackedValue);
            // Serial.print(' ');
            // memcpy(&bigEndianValue, &packet[36], 4);
            // Serial.print(ntohf(bigEndianValue));

            // Serial.print(' ');
            // memcpy(&bigEndianValue, &packet[40], 4);
            // Serial.print(ntohf(bigEndianValue));

            // Serial.print(' ');
            // memcpy(&bigEndianValue, &packet[44], 4);
            // Serial.print(ntohf(bigEndianValue));

            // Serial.print(' ');
            // memcpy(&bigEndianValue, &packet[56], 4);
            // Serial.print(ntohf(bigEndianValue));

            // Serial.print(' ');
            // memcpy(&bigEndianValue, &packet[60], 4);
            // Serial.println(ntohf(bigEndianValue));
        }
    }
}

float ntohn(uint32_t nf)
{
    int x;
    nf = ntohl(nf);
    memcpy(&x, &nf, sizeof(float));
    return x;
}

unsigned char reversechar(unsigned char c)
{
    c = (c & 0x55) << 1 | (c & 0xAA) >> 1;
    c = (c & 0x33) << 2 | (c & 0xCC) >> 2;
    c = (c & 0x0F) << 4 | (c & 0xF0) >> 4;
    return c;
}

unsigned char reverse(unsigned char b)
{
    return (unsigned char)(((b * 0x0802U & 0x22110U) | (b * 0x8020U & 0x88440U)) * 0x10101U >> 16);
}