#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char *ssid = "16988";
const char *password = "bric16988";

WiFiUDP Udp;
unsigned int port = 9999;
char packet[UDP_TX_PACKET_MAX_SIZE];
char null_packet[UDP_TX_PACKET_MAX_SIZE];

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

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
}

int orientation_X;

void loop()
{
    int packetSize = Udp.parsePacket();
    if (packetSize)
    {
        memcpy(&packet, &null_packet, UDP_TX_PACKET_MAX_SIZE);

        int len = Udp.read(packet, UDP_TX_PACKET_MAX_SIZE);

        if (len > 0)
        {
            Serial.printf("UDP packet contents: %s\n", packet);
            if (strcmp(packet, "off") == 0) // 如果收到on
            {
                digitalWrite(LED_BUILTIN, HIGH);
            }
            else if (strcmp(packet, "on") == 0) // 如果收到Button
            {
                digitalWrite(LED_BUILTIN, LOW);
            }
            // int32_t bigEndianValue;
            // memcpy(&bigEndianValue, &packet[36], 4);
            // orientation_X = ntohf(bigEndianValue);
            // orientation_X = orientation_X+180;
            // Serial.printf("orientation_X: %1.4f\n",orientation_X);
            // Serial.printf("orientation_X: %d\n",orientation_X);
            // analogWrite(D4,orientation_X*2048/360);
            // if(orientation_X>100){
            //     digitalWrite(D4, 1);
            // }else{
            //     digitalWrite(D4, 0);
            // }
        }
    }
}

float ntohf(uint32_t nf)
{
    float x;
    nf = ntohl(nf);
    memcpy(&x, &nf, sizeof(float));
    return x;
}