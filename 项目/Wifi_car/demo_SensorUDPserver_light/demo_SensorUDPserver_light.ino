#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include "MRUN_lib.h"

// 电机驱动引脚
int leftMotor1 = 0; // 前后轮子
int leftMotor2 = 5;
int rightMotor1 = 2; // 左右轮子
int rightMotor2 = 4;

// PSx摇杆
int Y_MAX = 255;
int Y_MID = 127;
int Y_MIN = 0;
int X_MAX = 255;
int X_MID = 127;
int X_MIN = 0;
int SILL = 2; // 偏移阈值

MRUN mrun;

#define SPEED_MAX 1023
#define SPEED_MID 512
#define SPEED_MIN 400
#define MAX_SPEED 1023
#define STOP 0

int RUN_SPEED = STOP; // 推进速度
int LR = STOP;        // 转向速度

// 获取mac地址
uint8_t MAC_array_STA[6];
uint8_t MAC_array_AP[6];
char MAC_char_STA[18];
char MAC_char_AP[18];

// 最大速度缓存值
uint16_t MAX_SPEED_INIT = 0;

const char *ssid = "M2";
const char *password = "50360891111";

String bod_type = "Nodemcu%20base%20v1";

WiFiUDP Udp;
unsigned int port = 9999;
char packet[UDP_TX_PACKET_MAX_SIZE];
char null_packet[UDP_TX_PACKET_MAX_SIZE];

int EMPTY_PACKAGE_NUM = 0; // 空包阈值，防止信号中断抽风

void setup()
{
    Serial.begin(115200);
    Serial.println();

    // 获取mac地址
    WiFi.macAddress(MAC_array_STA);
    WiFi.softAPmacAddress(MAC_array_AP);
    for (int i = 0; i < sizeof(MAC_array_STA); ++i)
    {
        sprintf(MAC_char_STA, "%s%02x:", MAC_char_STA, MAC_array_STA[i]);
        sprintf(MAC_char_AP, "%s%02x:", MAC_char_AP, MAC_array_AP[i]);
    }
    Serial.printf("MAC STA: ");
    Serial.println(MAC_char_STA);
    Serial.printf("MAC AP: ");
    Serial.println(MAC_char_AP);

    mrun.config(leftMotor1, leftMotor2, rightMotor1, rightMotor2, Y_MAX, Y_MID, Y_MIN, X_MAX, X_MID, X_MIN, SILL);

    Serial.printf("Connecting to %s ", ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("Connection Successful");

    WiFiClient client;

    HTTPClient http;

    Serial.print("[HTTP] begin...\n");
    String mac_add(MAC_char_AP);
    String mac_add_new = mac_add.substring(0, 17);
    String ip_add(WiFi.localIP().toString());
    Serial.print("http://siglephp.herokuapp.com/iot?msg={%22mac%22:%22" + mac_add_new + "%22,%22ip%22:%22" + ip_add + "%22,%22tag%22:%22" + bod_type + "%22}\n");
    if (http.begin(client, "http://siglephp.herokuapp.com/iot?msg={%22mac%22:%22" + mac_add_new + "%22,%22ip%22:%22" + ip_add + "%22,%22tag%22:%22" + bod_type + "%22}"))
    { // HTTP

        Serial.print("[HTTP] GET...\n");
        // start connection and send HTTP header
        int httpCode = http.GET();

        // httpCode will be negative on error
        if (httpCode > 0)
        {
            // HTTP header has been send and Server response header has been handled
            Serial.printf("[HTTP] GET... code: %d\n", httpCode);

            // file found at server
            if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
            {
                String payload = http.getString();
                Serial.println(payload);
            }
        }
        else
        {
            Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

        http.end();
    }
    else
    {
        Serial.printf("[HTTP} Unable to connect\n");
    }

    Udp.begin(port);
    Serial.printf("Listener started at IP % s, at port % d\n", WiFi.localIP().toString().c_str(), port);

    // pinMode(LED_BUILTIN, OUTPUT);
    // digitalWrite(LED_BUILTIN, HIGH);
    pinMode(D8, OUTPUT);
    pinMode(D7, OUTPUT);
    mrun.MAX_RUN_SPEED = 1023;
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
            EMPTY_PACKAGE_NUM = 0;
            Serial.printf("UDP packet contents: %s\n", packet);
            if (strcmp(packet, "S1:0") == 0) // 如果收到S1:0
            {
                digitalWrite(D7, HIGH); // OFF
            }
            else if (strcmp(packet, "S1:1") == 0)
            {
                digitalWrite(D7, LOW);
            }

            if (strcmp(packet, "S2:0") == 0) // 如果收到S2:0
            {
                digitalWrite(D8, HIGH); // OFF
            }
            else if (strcmp(packet, "S2:1") == 0)
            {
                digitalWrite(D8, LOW);
            }

            // 速度初始化
            // RUN_SPEED = STOP;
            // LR = STOP;

            if (strcmp(packet, "UP:1") == 0)
            {
                RUN_SPEED = MAX_SPEED;
            }
            else if (strcmp(packet, "DOWN:1") == 0)
            {
                RUN_SPEED = -MAX_SPEED;
            }
            else if (strcmp(packet, "UP:0") == 0 || strcmp(packet, "DOWN:0") == 0)
            {
                RUN_SPEED = STOP;
            }

            if (strcmp(packet, "LEFT:1") == 0 || strcmp(packet, "A:1") == 0)
            {
                LR = MAX_SPEED;
            }
            else if (strcmp(packet, "RIGHT:1") == 0 || strcmp(packet, "B:1") == 0)
            {
                LR = -MAX_SPEED;
            }
            else if (strcmp(packet, "LEFT:0") == 0 || strcmp(packet, "RIGHT:0") == 0 || strcmp(packet, "A:0") == 0 || strcmp(packet, "B:0") == 0)
            {
                LR = STOP;
            }

            if (LR < 0)
            {
                if (RUN_SPEED > STOP)
                {
                    // mrun.two(MAX_SPEED, STOP);
                    mrun.two(MAX_SPEED, STOP + MAX_SPEED_INIT - SPEED_MIN);
                }
                else if (RUN_SPEED < STOP)
                {
                    // mrun.two(STOP, -MAX_SPEED);
                    mrun.two(STOP - MAX_SPEED_INIT + SPEED_MIN, -MAX_SPEED);
                }
                else
                {
                    mrun.two(MAX_SPEED, -MAX_SPEED);
                }
            }
            else if (LR > 0)
            {
                if (RUN_SPEED > STOP)
                {
                    // mrun.two(STOP, MAX_SPEED);
                    mrun.two(STOP + MAX_SPEED_INIT - SPEED_MIN, MAX_SPEED);
                }
                else if (RUN_SPEED < STOP)
                {
                    // mrun.two(-MAX_SPEED, STOP);
                    mrun.two(-MAX_SPEED, STOP - MAX_SPEED_INIT + SPEED_MIN);
                }
                else
                {
                    mrun.two(-MAX_SPEED, MAX_SPEED);
                }
            }
            else
            {
                if (RUN_SPEED > STOP)
                {
                    mrun.two(RUN_SPEED, RUN_SPEED);
                }
                else if (RUN_SPEED < STOP)
                {
                    mrun.two(RUN_SPEED, RUN_SPEED);
                }
                else
                {
                    mrun.two(STOP, STOP);
                }
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
        else
        {
            if (EMPTY_PACKAGE_NUM > 3e4)
            {
                mrun.two(STOP, STOP);
                return;
            }
            else
            {
                EMPTY_PACKAGE_NUM++;
            }
        }
    }
    else
    {
        if (EMPTY_PACKAGE_NUM > 3e4)
        {
            mrun.two(STOP, STOP);
            return;
        }
        else
        {
            EMPTY_PACKAGE_NUM++;
        }
    }
}

// float ntohf(uint32_t nf)
// {
//     float x;
//     nf = ntohl(nf);
//     memcpy(&x, &nf, sizeof(float));
//     return x;
// }