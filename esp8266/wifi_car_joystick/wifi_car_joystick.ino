/*

发送端

ESP8266通过AP模式接收UDP信号
另一台通过STA模式发送UDP信号
实现P2P通讯


Wii手柄接线 (esp8266的I2C是可以定义引脚的，默认的是SDA为4，SCL为5号引脚)
Top Left 		SCK		Digital Pin 1 (GPIO05)
Top Right		GND		Ground Pin
Bottom L.		+ V		5 Volt Pin
Bottom R.		SDA		Digital Pin 2 (GOIO04)

nunchuk_accelX()  最大值 506

*/

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char *ssid = "M4";
const char *password = "ifconfig";

WiFiUDP Udp;
unsigned int port = 9999;
char packet[6];
IPAddress apip(192, 168, 4, 1);

int test_num = 0; //发包计数器


#define x D4       //定义X轴对应的引脚为A5 
#define y D3       //定义y轴对应的引脚为A4
#define sw D2       //定义按键对应的引脚为2





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
    delay(10);
    if (nunchuk_read())
    {
        // Work with nunchuk_data
        packet[0] = nunchuk_joystickX()+123;
        packet[1] = nunchuk_joystickY()+126;
        packet[2] = nunchuk_buttonZ()+1;
        packet[3] = nunchuk_buttonC()+1;
        packet[4] = map(constrain(nunchuk_accelX(),-260,100),-260,100,1,255);
        packet[5] = map(constrain(nunchuk_accelY(),-200,160),-200,160,1,255);
        packet[6] = map(constrain(nunchuk_accelZ(),-200,160),-200,160,1,255);

        if(packet[4]==255 && packet[6]==255 && packet[6]==255){
            Serial.println("The read handle is abnormal, please reset");
            delay(1000);
            return;
        }

        Udp.beginPacket(apip, port);
        Udp.write(packet);
        Udp.endPacket();
        // Serial.printf("UDP packet contents: %d  %d\n", packet[0], test_num);
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

        Serial.printf(" test_num: %d ", test_num);
        test_num++;
    }

}