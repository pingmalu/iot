/*
 * IRremoteESP8266: IRrecvDemo - demonstrates receiving IR codes with IRrecv
 * This is very simple teaching code to show you how to use the library.
 * If you are trying to decode your Infra-Red remote(s) for later replay,
 * use the IRrecvDumpV2.ino example code instead of this.
 * An IR detector/demodulator must be connected to the input kRecvPin.
 * Copyright 2009 Ken Shirriff, http://arcfn.com
 * Example circuit diagram:
 *  https://github.com/crankyoldgit/IRremoteESP8266/wiki#ir-receiving
 * Changes:
 *   Version 0.2 June, 2017
 *     Changed GPIO pin to the same as other examples.
 *     Used our own method for printing a uint64_t.
 *     Changed the baud rate to 115200.
 *   Version 0.1 Sept, 2015
 *     Based on Ken Shirriff's IrsendDemo Version 0.1 July, 2009
 */

#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
#include <Servo.h>
// comment out this line, if you want to show logs:
#define NDEBUG

#ifdef NDEBUG
#define LOG(...)
#define LOGLN(...)
#else
#define LOG(...) Serial.print(__VA_ARGS__)
#define LOGLN(...) Serial.println(__VA_ARGS__)
#endif
Servo servo;

// 循环次数
int looptimes = 0;

//打开舵机
void open()
{
    servo.attach(D1);
    //you begin your own personal code for servo here
    LOGLN("open");
    digitalWrite(LED_BUILTIN, LOW); //把板载led点亮
    servo.write(180);
    delay(1000);
    servo.write(0);
    delay(1000);
}

// 匹配用户
void CheckUser(uint64_t id)
{
    // uint64_t user = 0x55168EAC0082;
    uint64_t user = 0xE0E0A659;
    // print() & println() can't handle printing long longs. (uint64_t)
    serialPrintUint64(id, HEX);
    if (user == id)
    {
        LOGLN("Hello Malu!");
        digitalWrite(LED_BUILTIN, LOW); //把板载led点亮
        open();
    }
}

// 休眠
void go_poweroff()
{
    LOGLN("go to sleep!!!");
    ESP.deepSleep(3e6); // 10 seconds
}

// An IR detector/demodulator is connected to GPIO pin 14(D5 on a NodeMCU
// board).
const uint16_t kRecvPin = 14;

IRrecv irrecv(kRecvPin);

decode_results results;

void setup()
{
    //********** CHANGE PIN FUNCTION  TO GPIO **********
    //GPIO 1 (TX) swap the pin to a GPIO.
    // pinMode(0, FUNCTION_3);
    // pinMode(2, FUNCTION_3);
    //GPIO 3 (RX) swap the pin to a GPIO.
    // pinMode(3, FUNCTION_3);
    //**************************************************
    //给D3引脚高电平，给rf522供电
    // pinMode(D3, OUTPUT);
    // digitalWrite(D3, HIGH);

    //先把板载led关闭
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);

    Serial.begin(115200);
    irrecv.enableIRIn(); // Start the receiver
    while (!Serial)      // Wait for the serial connection to be establised.
        delay(50);
    LOGLN();
    LOG("IRrecvDemo is now running and waiting for IR message on Pin ");
    LOGLN(kRecvPin);
}

void loop()
{
    delay(100);
    // 到时间进入休眠
    // if (looptimes > 1)
    // {
    //     go_poweroff();
    //     return;
    // }
    if (irrecv.decode(&results))
    {
        // print() & println() can't handle printing long longs. (uint64_t)
        // serialPrintUint64(results.value, HEX);
        CheckUser(results.value);
        LOGLN("|");
        irrecv.resume(); // Receive the next value
    }
    else
    {
        LOGLN("not found");
    }
    // 休眠计时器
    // looptimes++;
}