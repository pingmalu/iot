#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
// 3.32   11.248

const char *ssid = "wifi";
const char *password = "passwd";

WiFiUDP Udp;
unsigned int port = 9999;
char packet[UDP_TX_PACKET_MAX_SIZE];

void setup()
{
    Serial.begin(115200);
    Serial.println();

    Serial.printf("Connecting to %s ", ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("Connection Successful");
    Udp.begin(port);
    Serial.printf("Listener started at IP % s, at port % d\n", WiFi.localIP().toString().c_str(), port);
}

void loop()
{
    int packetSize = Udp.parsePacket();
    if (packetSize)
    {
        Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
        int len = Udp.read(packet, UDP_TX_PACKET_MAX_SIZE);
        // Serial.println(UDP_TX_PACKET_MAX_SIZE);


        if (len > 0)
        {
            int32_t bigEndianValue;
            // memcpy(&bigEndianValue, &packet[0], 4);
            // float theUnpackedValue = ntohf(bigEndianValue);
            // Serial.print(theUnpackedValue);
            // Serial.print(' ');
            memcpy(&bigEndianValue, &packet[36], 4);
            Serial.print(ntohf(bigEndianValue));

                        Serial.print(' ');
            memcpy(&bigEndianValue, &packet[40], 4);
            Serial.print(ntohf(bigEndianValue));

                        Serial.print(' ');
            memcpy(&bigEndianValue, &packet[44], 4);
            Serial.print(ntohf(bigEndianValue));

            Serial.print(' ');
            memcpy(&bigEndianValue, &packet[56], 4);
            Serial.print(ntohf(bigEndianValue));

            Serial.print(' ');
            memcpy(&bigEndianValue, &packet[60], 4);
            Serial.println(ntohf(bigEndianValue));
            // packet[len] = '\0';
            // Serial.println(len);
            // int value = packet[0]*256 + packet[1];
            // Serial.println(struct.unpack_from ('!f', packet, 0));
        }
        // Serial.printf("UDP packet contents: %f\n", packet);
    }
}

float ntohf(uint32_t nf)
{
   float x;
   nf = ntohl(nf);
   memcpy(&x, &nf, sizeof(float));
   return x;
}