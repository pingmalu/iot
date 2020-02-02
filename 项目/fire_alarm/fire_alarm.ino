#include <Arduino.h>

// comment out this line, if you want to show logs:
#define NDEBUG

#ifdef NDEBUG
#define LOG(...)
#define LOGLN(...)
#else
#define LOG(...) Serial.print(__VA_ARGS__)
#define LOGLN(...) Serial.println(__VA_ARGS__)
#endif

uint8_t buzzer_PIN = 11;       //设置蜂鸣器的数字引脚为8
uint8_t flame_sensor_PIN = 12; //设置火焰传感器的数字引脚为4
uint8_t flame_detected;       //定义一个变量
// int flame_sensor_A0 = 0;

// // 休眠
// void go_poweroff()
// {
//     LOGLN("go to sleep!!!");
//     ESP.deepSleep(10e6); // 10 seconds
// }

void setup() //初始化
{
    Serial.begin(115200);
    pinMode(buzzer_PIN, OUTPUT);      //将8号引脚设置为输出信号
    pinMode(LED_BUILTIN, OUTPUT);     //将13号引脚设置为输出信号
    pinMode(flame_sensor_PIN, INPUT); //将4号引脚设置为输入信号
    digitalWrite(buzzer_PIN, LOW);
}

void loop()
{
    flame_detected = digitalRead(flame_sensor_PIN);
    // flame_sensor_A0 = analogRead(A0);
    // LOGLN(flame_sensor_A0);
    // LOGLN(flame_detected);
    //读取火焰传感器的数字输出并将其存储在变量“flame_detected”中。
    if (flame_detected == 0)
    {
        LOGLN("Flame detected...! take action immediately.");
        digitalWrite(buzzer_PIN, HIGH);
        tone(buzzer_PIN, 1000); //将8号数字引脚的输出设置为1000Hz频率，蜂鸣器触发
        digitalWrite(LED_BUILTIN, HIGH);
    }
    else
    {
        LOGLN("No flame detected. stay cool");
        digitalWrite(buzzer_PIN, LOW);
        noTone(buzzer_PIN);
        digitalWrite(LED_BUILTIN, LOW);
    }
    delay(1000);
    // go_poweroff();
}