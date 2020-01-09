#include <Arduino.h>

int buzzer_PIN = 5;       //设置蜂鸣器的数字引脚为8
int flame_sensor_PIN = 4; //设置火焰传感器的数字引脚为4
int flame_detected;   //定义一个变量

void setup() //初始化
{
    Serial.begin(9600);
    pinMode(buzzer_PIN, OUTPUT);      //将8号引脚设置为输出信号
    pinMode(LED_BUILTIN, OUTPUT);         //将13号引脚设置为输出信号
    pinMode(flame_sensor_PIN, INPUT); //将4号引脚设置为输入信号
    digitalWrite(buzzer_PIN, LOW);
}

void loop()
{
    flame_detected = digitalRead(flame_sensor_PIN);
    //读取火焰传感器的数字输出并将其存储在变量“flame_detected”中。
    if (flame_detected == 1)
    {
        Serial.println("Flame detected...! take action immediately.");
        digitalWrite(buzzer_PIN, HIGH);
        tone(buzzer_PIN, 1000); //将8号数字引脚的输出设置为1000Hz频率，蜂鸣器触发
        Serial.println("Bi~");
        digitalWrite(LED_BUILTIN, HIGH);
    }
    else
    {
        Serial.println("No flame detected. stay cool");
        digitalWrite(buzzer_PIN, LOW);
        noTone(buzzer_PIN);
        digitalWrite(LED_BUILTIN, LOW);
    }
    delay(1000);
}