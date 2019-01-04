#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Servo.h>

const char *ssid = "16988";
const char *password = "bric16988";

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



int orientation_X;
int orientation_X_tmp;
int orientation_Y;
int orientation_Y_tmp;
int orientation_Z;
int32_t bigEndianValue;

void loop()
{
    int packetSize = Udp.parsePacket();
    if (packetSize)
    {
        int len = Udp.read(packet, UDP_TX_PACKET_MAX_SIZE);

        if (len > 0)
        {
            memcpy(&bigEndianValue, &packet[36], 4);
            orientation_X = ntohf(bigEndianValue)+180;

            memcpy(&bigEndianValue, &packet[40], 4);
            orientation_Y = ntohf(bigEndianValue)+180;

            memcpy(&bigEndianValue, &packet[44], 4);
            orientation_Z = ntohf(bigEndianValue)+180;
            // orientation_X = orientation_X*100;
            // Serial.printf("orientation_X: %1.4f\n",orientation_X);

            Serial.printf("orientation: %d %d %d\n",orientation_X,orientation_Y,orientation_Z);
            if(orientation_Z>150 && orientation_Z<210){
                myservo.write(90);
                myservo2.write(90);
            }else{

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