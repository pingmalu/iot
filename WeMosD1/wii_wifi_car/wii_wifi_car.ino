/*
 * wii鸡腿手柄通过wifi控制小车(D1版本)
 * BY: Malu
 * https://malu.me
 * 2019.2.12
 * 
 * 接收端
 * 
 * ESP8266通过AP模式接收UDP信号
 * 另一台通过STA模式发送UDP信号
 * 实现P2P通讯
*/

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char *ssid = "M4";
const char *password = "ifconfig";

WiFiUDP Udp;
unsigned int port = 9999;
char packet[6];

int test_num = 0; //发包计数器

// analogWrite(pin, value)  UNO:0-255  D1 ESP8266:0-1023
#define MAX_SPEED 1023 // PWM最大数值

// 电机速度定义
#define SPEED_1 100
#define SPEED_2 180
#define SPEED_3 255
#define SPEED_START 300  // 启动速度

int leftMotor1 = D3; // 前后轮子
int leftMotor2 = D4;
int rightMotor1 = D5; // 左右轮子
int rightMotor2 = D6;

int RUN_SPEED = 0; // 推进速度
int LR = 0;        // 转向速度

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

    // 电机驱动引脚初始化
    pinMode(leftMotor1, OUTPUT);
    pinMode(leftMotor2, OUTPUT);
    pinMode(rightMotor1, OUTPUT);
    pinMode(rightMotor2, OUTPUT);

    Udp.begin(port);
}

void loop()
{
    int packetSize = Udp.parsePacket();
    if (packetSize)
    {
        int len = Udp.read(packet, 7);
        // Serial.printf("Received %d bytes from %s, port %d, len %d, test_num %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort(), len, test_num);

        if (len > 0)
        {
            // packet[len] = 0; //末尾补0结束字符串
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
            Serial.printf(" D5:%d D6:%d D7:%d D8:%d ",D3,D4,D5,D6);  // D5:14 D6:12 D7:13 D8:15
            // Serial.println();
            test_num++;

            // 前进 170-220  后 23-70  左 29-70  右 170-238

            if ((int)packet[0] < 70)
            { // 左
                LR = map(constrain((int)packet[0], 29, 70), 29, 70, -MAX_SPEED, -SPEED_START);
            }
            else if ((int)packet[0] > 170)
            { // 右
                LR = map(constrain((int)packet[0], 170, 238), 170, 238, SPEED_START, MAX_SPEED);
            }
            else
            {
                LR = 0;
            }

            if ((int)packet[1] > 170)
            { // 前
                RUN_SPEED = map(constrain((int)packet[1], 170, 220), 170, 220, SPEED_START, MAX_SPEED);
                RUN_SPEED += abs(LR);
            }
            else if ((int)packet[1] < 70)
            { // 后
                RUN_SPEED = map(constrain((int)packet[1], 23, 70), 23, 70, -MAX_SPEED, -SPEED_START);
                RUN_SPEED -= abs(LR);
            }
            else
            {
                RUN_SPEED = 0;
                RUN_SPEED += abs(LR);
            }

            if ((int)packet[3] == 2 && (int)packet[2] == 1)
            { // 前
                RUN_SPEED = MAX_SPEED;
            }
            else if ((int)packet[3] == 1 && (int)packet[2] == 2)
            { // 后
                RUN_SPEED = -MAX_SPEED;
            }

            Mrun_one(RUN_SPEED, leftMotor1, leftMotor2);
            Mrun_one(LR, rightMotor1, rightMotor2);
            // Serial.printf(" RUN_SPEED:%d LR:%d \n",RUN_SPEED,LR);
            Serial.println();
        }
    }
}

void Mrun_one(int v, int M1, int M2)
{
    int v_abs = abs(v);
    //   v_abs = v_abs<MAX_SPEED?v_abs:MAX_SPEED;
    v_abs = constrain(v_abs, 0, MAX_SPEED);
    if (v > 0)
    {
        digitalWrite(M1, LOW);
        analogWrite(M2, v_abs);
        Serial.printf(" %d ", v_abs);
    }
    else if (v < 0)
    {
        digitalWrite(M1, HIGH);
        analogWrite(M2, MAX_SPEED - v_abs);
        Serial.printf(" %d ", -v_abs);
    }
    else
    {
        digitalWrite(M1, LOW);
        digitalWrite(M2, LOW);
    }
}

void Mrun(int vl = 0, int vr = 0)
{
    Mrun_one(vl, leftMotor1, leftMotor2);
    Mrun_one(vr, rightMotor1, rightMotor2);
    Serial.printf(" L:%d R:%d \n", vl, vr);
}
