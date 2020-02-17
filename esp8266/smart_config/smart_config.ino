#include <ESP8266WiFi.h>

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
    Serial.begin(115200);     //打开串口
    pinMode(FlashPin, INPUT); //设置管脚为输入
    pinMode(LedPin, OUTPUT);

    if (!autoConfig())
    {
        Serial.println("Start module");
        smartConfig();
    }

    digitalWrite(LedPin, HIGH);
}

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
}