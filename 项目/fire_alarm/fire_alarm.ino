#include <Arduino.h>

int buzzer = 8;       //设置蜂鸣器的数字引脚为8
int LED = 13;         //设置LED灯的数字引脚为13
int flame_sensor = 4; //设置火焰传感器的数字引脚为4
int flame_detected;   //定义一个变量

void setup() //初始化
{
    Serial.begin(9600);
    pinMode(buzzer, OUTPUT);      //将8号引脚设置为输出信号
    pinMode(LED, OUTPUT);         //将13号引脚设置为输出信号
    pinMode(flame_sensor, INPUT); //将4号引脚设置为输入信号
    digitalWrite(buzzer, LOW);
}

void loop()
{
    flame_detected = digitalRead(flame_sensor);
    //读取火焰传感器的数字输出并将其存储在变量“flame_detected”中。
    if (flame_detected == 1)
    {
        Serial.println("Flame detected...! take action immediately.");
        digitalWrite(buzzer, HIGH);
        tone(8, 1000); //将8号数字引脚的输出设置为1000Hz频率，蜂鸣器触发
        Serial.println("Bi~");
        digitalWrite(LED, HIGH);
    }
    else
    {
        Serial.println("No flame detected. stay cool");
        digitalWrite(buzzer, LOW);
        noTone(8);
        digitalWrite(LED, LOW);
    }
    delay(1000);
}