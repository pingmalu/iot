#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "MRUN_lib.h"

// 电机驱动引脚
int leftMotor1 = 2; // 前后轮子
int leftMotor2 = 4;
int rightMotor1 = 16; // 左右轮子
int rightMotor2 = 17;

// PSx摇杆
int Y_MAX = 255;
int Y_MID = 127;
int Y_MIN = 0;
int X_MAX = 255;
int X_MID = 127;
int X_MIN = 0;
int SILL = 2; // 偏移阈值

MRUN mrun;

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

    mrun.config(leftMotor1, leftMotor2, rightMotor1, rightMotor2, Y_MAX, Y_MID, Y_MIN, X_MAX, X_MID, X_MIN, SILL);

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
            if (strcmp(packet, "S1:0") == 0) // 如果收到S1:0
            {
                digitalWrite(LED_BUILTIN, HIGH); // OFF
            }
            else if (strcmp(packet, "S1:1") == 0)
            {
                digitalWrite(LED_BUILTIN, LOW);
            }
            else if (strcmp(packet, "UP:1") == 0)
            {
                mrun.two(500, 500);
            }
            else if (strcmp(packet, "UP:0") == 0)
            {
                mrun.two(0, 0);
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