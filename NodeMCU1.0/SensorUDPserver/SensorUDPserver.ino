/**
 * 方向感应器控制舵机
 * BY: Malu
 * https://malu.me
 * 2019.01.07
 */

// 传感器偏移位对应表：
// 0       acceleration 加速度X
// 4       acceleration 加速度Y
// 8       acceleration 加速度Z
// 12      gravity 重力X
// 16      gravity 重力Y
// 20      gravity 重力Z
// 24      rotation 旋转角度X
// 28      rotation 旋转角度Y
// 32      rotation 旋转角度Z
// 36      orientation 方向X
// 40      orientation 方向Y
// 44      orientation 方向Z
// 48      ambient-light 环境光
// 52      proximity 距离
// 56      
// 60      
// 64      按钮1
// 68      按钮2
// 72      按钮3
// 76      按钮4
// 80      按钮5
// 84      按钮6
// 88      按钮7
// 92      按钮8

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Servo.h>

const char *ssid = "16988";
const char *password = "bric16988";

IPAddress ip(192, 168, 11, 213);
IPAddress gateway(192, 168, 11, 2);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(8, 8, 8, 8);

WiFiUDP Udp;
unsigned int port = 9999;
char packet[UDP_TX_PACKET_MAX_SIZE];

Servo myservo;
Servo myservo2;
int servoPin = D4;  // 舵机针脚，棕色为地，红色为电源正，橙色为信号线
int servoPin2 = D3;

void setup()
{
    Serial.begin(115200);
    Serial.println();

    Serial.printf("Connecting to %s ", ssid);
    WiFi.mode(WIFI_STA);
    WiFi.config(ip, gateway, subnet, dns);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("Connection Successful");
    Udp.begin(port);
    Serial.printf("Listener started at IP % s, at port % d\n", WiFi.localIP().toString().c_str(), port);

    myservo.attach(servoPin);
    myservo2.attach(servoPin2);
    // pinMode(D4, OUTPUT);
    // digitalWrite(D4, 0);
}

int orientation_X;
int orientation_X_tmp = 0;
int servo_X;
int orientation_Y;
int orientation_Y_tmp = 0;
int servo_Y;
int orientation_Z;
int32_t bigEndianValue;
bool button_1 = false;
bool button_tag = false;
bool button_tag_X = false;
bool button_tag_Y = false;

// 计数器,缓冲器减缓抖动
int cut_count = 10;
int Buffer_count = 0;
int Buffer_X = 0;
int Buffer_Y = 0;

void loop()
{
    int packetSize = Udp.parsePacket();
    if (packetSize)
    {
        int len = Udp.read(packet, UDP_TX_PACKET_MAX_SIZE);

        if (len > 0)
        {
            memcpy(&bigEndianValue, &packet[72], 4);
            if(ntohf(bigEndianValue)==1){
                button_tag = false;
            }
            memcpy(&bigEndianValue, &packet[76], 4);
            if(ntohf(bigEndianValue)==1){
                button_tag = true;
            }

            memcpy(&bigEndianValue, &packet[80], 4);
            if(ntohf(bigEndianValue)==1){
                button_tag_X = false;
            }
            memcpy(&bigEndianValue, &packet[84], 4);
            if(ntohf(bigEndianValue)==1){
                button_tag_X = true;
            }

            memcpy(&bigEndianValue, &packet[88], 4);
            if(ntohf(bigEndianValue)==1){
                button_tag_Y = false;
            }
            memcpy(&bigEndianValue, &packet[92], 4);
            if(ntohf(bigEndianValue)==1){
                button_tag_Y = true;
            }
            
            memcpy(&bigEndianValue, &packet[40], 4);
            orientation_Y = ntohf(bigEndianValue)+180;

            if(!button_tag){
                memcpy(&bigEndianValue, &packet[36], 4);
                orientation_X = ntohf(bigEndianValue)+180;

                memcpy(&bigEndianValue, &packet[44], 4);
                orientation_Z = ntohf(bigEndianValue)+180;
            }else{
                memcpy(&bigEndianValue, &packet[44], 4);
                orientation_X = ntohf(bigEndianValue)+180;

                // memcpy(&bigEndianValue, &packet[36], 4);
                // orientation_Z = ntohf(bigEndianValue)+180;
                orientation_Z = 33;
            }

            memcpy(&bigEndianValue, &packet[64], 4);
            button_1 = ntohf(bigEndianValue)==0?false:true;
            // orientation_X = orientation_X*100;
            // Serial.printf("orientation_X: %1.4f\n",orientation_X);

            if(orientation_Z>359+2-30 || orientation_Z<2+30 || button_1){
                servo_X = 90;
                orientation_X_tmp = orientation_X;
                servo_Y = 90;
                orientation_Y_tmp = orientation_Y;
            }else{
                servo_X=orientation_X-orientation_X_tmp;
                if(servo_X<0){
                    servo_X=359+servo_X;
                }
                if(servo_X>90 && servo_X<180){
                    servo_X = 0;
                }else if(servo_X>=180 && servo_X<270){
                    servo_X = 180;
                }else if(servo_X>=270){
                    servo_X = 450-servo_X;
                }else{
                    servo_X = 90-servo_X;
                }
                if(button_tag_X){
                    servo_X = 180-servo_X;  //舵机反转
                }

                servo_Y=orientation_Y-orientation_Y_tmp;
                if(servo_Y<0){
                    servo_Y=359+servo_Y;
                }
                if(servo_Y>90 && servo_Y<180){
                    servo_Y = 0;
                }else if(servo_Y>=180 && servo_Y<270){
                    servo_Y = 180;
                }else if(servo_Y>=270){
                    servo_Y = 450-servo_Y;
                }else{
                    servo_Y = 90-servo_Y;
                }
                if(button_tag_Y){
                    servo_Y = 180-servo_Y;  //舵机反转
                }
            }

            Buffer_count++;
            Buffer_X+=servo_X;
            Buffer_Y+=servo_Y;
            if(Buffer_count>=cut_count){
                myservo.write(Buffer_X/cut_count);
                myservo2.write(Buffer_Y/cut_count);
                Serial.printf("orientation: %d %d %d",orientation_X,orientation_Y,orientation_Z);
                Serial.printf("  but:%d %d",button_1,button_tag);
                Serial.printf("  servo:%d %d",Buffer_X/cut_count,Buffer_Y/cut_count);
                Serial.printf("  tmp:%d %d\n",orientation_X_tmp,orientation_Y_tmp);
                Buffer_count=0;
                Buffer_X=0;
                Buffer_Y=0;
            }
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