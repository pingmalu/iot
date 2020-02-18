#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
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

// 最大速度缓存值
uint16_t MAX_SPEED_INIT = 0;

WiFiUDP Udp;
unsigned int port = 9999;
char packet[UDP_TX_PACKET_MAX_SIZE];
char null_packet[UDP_TX_PACKET_MAX_SIZE];

int EMPTY_PACKAGE_NUM = 0; // 空包阈值，防止信号中断抽风



uint8_t FlashPin = 0; //Flash脚
uint8_t LedPin = 2;   //Led脚
int count = 0;

bool autoConfig()
{
    WiFi.begin();
    for (int i = 0; i < 20; i++)
    {
        int wstatus = WiFi.status();
        if (wstatus == WL_CONNECTED)
        {
            Serial.println("AutoConfig Success");
            Serial.printf("SSID:%s\r\n", WiFi.SSID().c_str());
            Serial.printf("PSW:%s\r\n", WiFi.psk().c_str());

            Serial.println("WiFi Connected.");
            Serial.print("IP Address: ");
            Serial.println(WiFi.localIP());
            WiFi.printDiag(Serial);
            return true;
            //break;
        }
        else
        {
            Serial.print("AutoConfig Waiting......");
            Serial.println(wstatus);
            delay(1000);
        }
    }
    Serial.println("AutoConfig Faild!");
    return false;
    //WiFi.printDiag(Serial);
}
void smartConfig()
{
    WiFi.mode(WIFI_STA);
    Serial.println("\r\nWait for Smartconfig");
    WiFi.beginSmartConfig();
    while (1)
    {
        Serial.print(".");
        if (WiFi.smartConfigDone())
        {
            Serial.println("SmartConfig Success");
            Serial.printf("SSID:%s\r\n", WiFi.SSID().c_str());
            Serial.printf("PSW:%s\r\n", WiFi.psk().c_str());
            WiFi.setAutoConnect(true); // 设置自动连接

            /* Wait for WiFi to connect to AP */
            Serial.println("Waiting for WiFi");
            while (WiFi.status() != WL_CONNECTED)
            {
                delay(500);
                Serial.print(".");
            }
            Serial.println("WiFi Connected.");
            Serial.print("IP Address: ");
            Serial.println(WiFi.localIP());
            break;
        }
        delay(500); // 这个地方一定要加延时，否则极易崩溃重启
    }
}


void setup()
{
    Serial.begin(115200);
    Serial.println();

    mrun.config(leftMotor1, leftMotor2, rightMotor1, rightMotor2, Y_MAX, Y_MID, Y_MIN, X_MAX, X_MID, X_MIN, SILL);

    pinMode(FlashPin, INPUT); //设置管脚为输入
    pinMode(LedPin, OUTPUT);

    if (!autoConfig())
    {
        Serial.println("Start module");
        smartConfig();
    }

    digitalWrite(LedPin, HIGH);

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
    while (digitalRead(FlashPin) == LOW)
    {
        delay(200);
        count++;
        if (count >= 20)
        { //>=4s
            digitalWrite(LedPin, LOW);
            delay(300);
            digitalWrite(LedPin, HIGH);
            delay(100);
            digitalWrite(LedPin, LOW);
            delay(300);
            digitalWrite(LedPin, HIGH);

            ESP.eraseConfig();
            delay(1000);
            ESP.reset();
            ESP.restart();
            //Serial.println("Long Key");
            break;
        }
    }
    count = 0;
    digitalWrite(LedPin, HIGH);
    delay(200); //nothing to do just delay


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