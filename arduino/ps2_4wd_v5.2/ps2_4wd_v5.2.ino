/*
 * ps2手柄控制4轮驱动小车  5.0
 * 4.2版本加入舵机摇杆控制
 * 4.3版本加入坦克模式切换蜂鸣模拟
 * 5.0版本修改MRUN库，增加最大速度控制
 * 5.1版本专门适配迷你四轮驱动小车n20电机驱动
 * 5.2版本专门适配mg996舵机四轮驱动大车，推土机
 * BY: Malu
 * https://malu.me
 * 2019.11.23
 */

#ifndef ESP8266
#include <avr/sleep.h>
#define ISESP8266 false
#else
#define ISESP8266 true
#endif
#include <PS2X_lib.h> //for v1.6

#include "MRUN_lib.h"

#include <Servo.h> // 舵机控制
Servo myservo1;    // 创建舵机对象
Servo myservo2;    // 创建舵机对象

// NODEMCU版本引脚
int leftMotor1 = D5; // 前后轮子
int leftMotor2 = D6;
int rightMotor1 = D7; // 左右轮子
int rightMotor2 = D8;
// 接收机引脚
#define PS2_DAT D1 //14
#define PS2_CMD D2 //15
#define PS2_CS D3  //16
#define PS2_CLK D4 //17
// 舵机引脚
#define SERVO_PIN_1 16
#define SERVO_PIN_2 3

// PS2X摇杆
int Y_MAX = 255;
int Y_MID = 127;
int Y_MIN = 0;
int X_MAX = 255;
int X_MID = 128;
int X_MIN = 0;
int SILL = 6; // 偏移阈值

// 舵机角度值
int lx;
// int ly;

// // D1版本引脚
// int leftMotor1 = D2; // 前后轮子
// int leftMotor2 = D3;
// int rightMotor1 = D4; // 左右轮子
// int rightMotor2 = D5;
// // 接收机引脚
// #define PS2_DAT D13 //14
// #define PS2_CMD D12 //15
// #define PS2_CS D11  //16
// #define PS2_CLK D10 //17

// // UNO版本引脚
// int leftMotor1 = 2; // 左边轮子
// int leftMotor2 = 3;
// int rightMotor1 = 4; // 右边轮子
// int rightMotor2 = 5;
// // 接收机引脚
// #define PS2_DAT 13 //14
// #define PS2_CMD 11 //15
// #define PS2_CS 10  //16
// #define PS2_CLK 12 //17
// // 舵机引脚
// #define SERVO_PIN_1 9
// #define SERVO_PIN_2 8

// // UNO 2.4g_tank 引脚
// int leftMotor1 = 4; // 左边轮子
// int leftMotor2 = 5;
// int rightMotor1 = 7; // 右边轮子
// int rightMotor2 = 6;
// // 接收机引脚
// #define PS2_DAT 11 //14
// #define PS2_CMD 10 //15
// #define PS2_CS 9   //16
// #define PS2_CLK 8  //17

// 驾驶定义
#define STOP 0
#define FORWARD 1
#define BACKWARD 2
#define TURNLEFT 3
#define TURNRIGHT 4

// 全局驾驶变量
bool TANK_MOD = true;

/******************************************************************
   select modes of PS2 controller:
     - pressures = analog reading of push-butttons
     - rumble    = motor rumbling
   uncomment 1 of the lines for each mode selection
 ******************************************************************/
#define pressures false
#define rumble false

PS2X ps2x; // create PS2 Controller Class
MRUN mrun;

// byte type = 0;

// analogWrite(pin, value)  UNO:0-255  D1 ESP8266:0-1023
#ifndef ESP8266
#define MAX_SPEED 255
#else
#define MAX_SPEED 1023
#endif

int RUN_SPEED = 0; // 推进速度
int LR = 0;        // 转向速度
// int START_TAG = 0;

unsigned long starttime;
unsigned long looptime;

void go_poweroff()
{
    Serial.print("go to sleep!!!");
    delay(1000);

#ifdef ESP8266
    ESP.deepSleep(30000e6);
#else
    pinMode(13, OUTPUT);   // UNO板载LED关闭
    digitalWrite(13, LOW); // UNO板载LED关闭
    // 采用“Power-down”睡眠模式
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    // 启用睡眠模式
    sleep_enable();
    // 进入睡眠模式
    sleep_cpu();
#endif
}

void setup()
{
    // Serial.begin(115200);

    int error = 0;
    do
    {
        error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_CS, PS2_DAT, pressures, rumble);
        if (error == 0)
        {
            break;
        }
        else
        {
            Serial.print(" uptime:");
            Serial.print(millis());
            Serial.print(" ");
            if (millis() > 600000) // 启动时检测不到手柄连接器，超过600秒，进入睡眠
            {
                go_poweroff();
            }
            delay(100);
        }
    } while (1);

    // 电机驱动引脚初始化
    pinMode(leftMotor1, OUTPUT);
    pinMode(leftMotor2, OUTPUT);
    pinMode(rightMotor1, OUTPUT);
    pinMode(rightMotor2, OUTPUT);

    // 舵机引脚初始化
    myservo1.attach(SERVO_PIN_1);
    myservo2.attach(SERVO_PIN_2);

    mrun.config(leftMotor1, leftMotor2, rightMotor1, rightMotor2, Y_MAX, Y_MID, Y_MIN, X_MAX, X_MID, X_MIN, SILL);

    // 初始最大速度
    mrun.MAX_RUN_SPEED = 512;

    starttime = millis();

    // type = ps2x.readType();
    // switch (type)
    // {
    // case 0:
    //     Serial.println("Unknown Controller type");
    //     break;
    // case 1:
    //     Serial.println("DualShock Controller Found");
    //     break;
    // case 2:
    //     Serial.println("GuitarHero Controller Found");
    //     break;
    // }
}

void loop()
{
    // if (ps2x.Button(PSB_START)){
    //     Serial.println("Start is being held");
    //     START_TAG = 1;
    // }

    // if(START_TAG == 0){
    //     delay(10);
    //     return;
    // }

    // 速度初始化
    RUN_SPEED = STOP;
    LR = STOP;

    ps2x.read_gamepad(false, 0); //read controller and set large motor to spin at 'vibrate' speed

    // SELECT 切换坦克模式
    if (ps2x.ButtonReleased(PSB_SELECT))
    {
        Serial.println("PSB_SELECT Button Released!");
        TANK_MOD = TANK_MOD ? false : true;
        if (TANK_MOD)
        {
            mrun.tank_v2(133, 133);
            delay(100);
            mrun.tank_v2(128, 128);
            delay(100);
            mrun.tank_v2(133, 133);
            delay(100);
        }
    }

    if (ps2x.Button(PSB_SELECT))
        Serial.println("Select is being held");
    if (ps2x.Button(PSB_START))
        Serial.println("PSB_START is being held");

    // 左边按键群
    if (ps2x.Button(PSB_PAD_UP))
    { //will be TRUE as long as button is pressed
        Serial.println("Up held this hard: ");
        RUN_SPEED = MAX_SPEED;
    }
    else if (ps2x.ButtonReleased(PSB_PAD_UP))
    {
        Serial.println("Up Button Released!");
        RUN_SPEED = STOP;
    }

    if (ps2x.Button(PSB_PAD_DOWN))
    {
        Serial.println("DOWN held this hard: ");
        RUN_SPEED = -MAX_SPEED;
    }
    else if (ps2x.ButtonReleased(PSB_PAD_DOWN))
    {
        Serial.println("DOWN Button Released!");
        RUN_SPEED = STOP;
    }

    if (ps2x.Button(PSB_PAD_RIGHT))
    {
        Serial.println("Right held this hard: ");
        LR = -MAX_SPEED;
    }
    else if (ps2x.ButtonReleased(PSB_PAD_RIGHT))
    {
        Serial.println("Right Button Released!");
        LR = STOP;
    }

    if (ps2x.Button(PSB_PAD_LEFT))
    {
        Serial.println("LEFT held this hard: ");
        LR = MAX_SPEED;
    }
    else if (ps2x.ButtonReleased(PSB_PAD_LEFT))
    {
        Serial.println("LEFT Button Released!");
        LR = STOP;
    }

    // 右边按键群
    if (ps2x.Button(PSB_TRIANGLE))
    { //will be TRUE as long as button is pressed
        Serial.println("Triangle pressed");
        RUN_SPEED = MAX_SPEED;
    }
    else if (ps2x.ButtonReleased(PSB_TRIANGLE))
    {
        Serial.println("Triangle Button Released!");
        RUN_SPEED = STOP;
    }

    if (ps2x.Button(PSB_CROSS))
    {
        Serial.println("PSB_CROSS pressed");
        RUN_SPEED = -MAX_SPEED;
    }
    else if (ps2x.ButtonReleased(PSB_CROSS))
    {
        Serial.println("PSB_CROSS Button Released!");
        RUN_SPEED = STOP;
    }

    if (ps2x.Button(PSB_CIRCLE)) // 右
    {
        Serial.println("PSB_CIRCLE pressed");
        LR = -MAX_SPEED;
    }
    else if (ps2x.ButtonReleased(PSB_CIRCLE))
    {
        Serial.println("PSB_CIRCLE Button Released!");
        LR = STOP;
    }

    if (ps2x.Button(PSB_SQUARE)) // 左
    {
        Serial.println("PSB_SQUARE pressed");
        LR = MAX_SPEED;
    }
    else if (ps2x.ButtonReleased(PSB_SQUARE))
    {
        Serial.println("PSB_SQUARE Button Released!");
        LR = STOP;
    }

    // 翻斗
    if (ps2x.Button(PSB_R1)) // 右
    {
        Serial.println("PSB_R1 pressed");
        myservo1.write(0);
        delay(10);
        return;
    }
    else if (ps2x.ButtonReleased(PSB_R1))
    {
        Serial.println("PSB_R1 Button Released!");
        myservo1.write(170);
    }

    // // 上按键加速，R1,L1
    // if (ps2x.Button(PSB_R1)) // 右
    // {
    //     Serial.println("PSB_R1 pressed");
    //     mrun.MAX_RUN_SPEED = 512; // 加速
    // }
    // else if (ps2x.ButtonReleased(PSB_R1))
    // {
    //     Serial.println("PSB_R1 Button Released!");
    //     mrun.MAX_RUN_SPEED = 250; // 加速释放
    // }

    // if (ps2x.Button(PSB_L1)) // 左
    // {
    //     Serial.println("PSB_L1 pressed");
    //     mrun.MAX_RUN_SPEED = 200;
    // }
    // else if (ps2x.ButtonReleased(PSB_L1))
    // {
    //     Serial.println("PSB_L1 Button Released!");
    //     mrun.MAX_RUN_SPEED = 250; // 加速释放
    // }

    // 上按键加速，R2,L2
    if (ps2x.Button(PSB_R2)) // 右
    {
        Serial.println("PSB_R2 pressed");
        mrun.MAX_RUN_SPEED = 1023; // 加速
    }
    else if (ps2x.ButtonReleased(PSB_R2))
    {
        Serial.println("PSB_R2 Button Released!");
        mrun.MAX_RUN_SPEED = 512; // 加速释放
    }

    if (ps2x.Button(PSB_L2)) // 左
    {
        Serial.println("PSB_L2 pressed");
        mrun.MAX_RUN_SPEED = 1023; // 加速
    }
    else if (ps2x.ButtonReleased(PSB_L2))
    {
        Serial.println("PSB_L2 Button Released!");
        mrun.MAX_RUN_SPEED = 512; // 加速释放
    }

    // // 快速旋转1s
    // if (ps2x.ButtonReleased(PSB_L3))
    // {
    //     Serial.println("PSB_L3 Button Released!");
    //     mrun.MAX_RUN_SPEED = 512;
    //     int tmp_speed = 255;
    //     mrun.two(-tmp_speed, tmp_speed);
    //     delay(300);
    //     return;
    // }
    // 高速锁定
    if (ps2x.ButtonReleased(PSB_R3))
    {
        Serial.println("PSB_R3 Button Released!");
        mrun.MAX_RUN_SPEED = 1023;
        return;
    }

    if (RUN_SPEED == STOP && LR == STOP) // 在按键全部释放
    {
        if (TANK_MOD)
        {
            RUN_SPEED = map(constrain((int)ps2x.Analog(PSS_LY), 0, 254), 0, 254, MAX_SPEED, -MAX_SPEED);
            LR = map(constrain((int)ps2x.Analog(PSS_RY), 0, 254), 0, 254, MAX_SPEED, -MAX_SPEED);
            mrun.two(RUN_SPEED, LR);
        }
        else
        {
            // 右摇杆
            RUN_SPEED = map(constrain((int)ps2x.Analog(PSS_RY), 0, 255), 0, 255, 255, 0);
            LR = map(constrain((int)ps2x.Analog(PSS_RX), 0, 255), 0, 255, 0, 255);

            if (RUN_SPEED == 128 && LR == 128) // R摇杆不在控制
            {
                // 左摇杆
                // RUN_SPEED = map(constrain((int)ps2x.Analog(PSS_LY), 0, 255), 0, 255, 255, 0);
                // LR = map(constrain((int)ps2x.Analog(PSS_LX), 0, 255), 0, 255, 0, 255);
                // mrun.tank_v2(RUN_SPEED, LR);

                // 摇杆中位时舵机打到170度,上下都可控制
                int LYnum = constrain((int)ps2x.Analog(PSS_LY), 0, 255);
                if (LYnum < Y_MID)
                {
                    RUN_SPEED = map(LYnum, Y_MID, 0, 170, 0);
                }
                else
                {
                    RUN_SPEED = map(LYnum, Y_MID, 255, 170, 0);
                }
                LR = map(constrain((int)ps2x.Analog(PSS_LX), 0, 255), 0, 255, 0, 180);
                myservo1.write(RUN_SPEED);
                myservo2.write(LR);
                mrun.tank_v2(Y_MID, X_MID);
            }
            else
            {
                mrun.tank(RUN_SPEED, LR);
            }
        }
    }
    else
    {
        // 坦克坐标变换
        if (LR < 0)
        {
            if (RUN_SPEED > STOP)
            {
                mrun.two(MAX_SPEED, STOP);
            }
            else if (RUN_SPEED < STOP)
            {
                mrun.two(STOP, -MAX_SPEED);
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
                mrun.two(STOP, MAX_SPEED);
            }
            else if (RUN_SPEED < STOP)
            {
                mrun.two(-MAX_SPEED, STOP);
            }
            else
            {
                mrun.two(-MAX_SPEED, MAX_SPEED);
            }
        }
        else
        {
            mrun.two(RUN_SPEED, RUN_SPEED);
        }
    }

    /*
    if (ps2x.Button(PSB_L2))
        Serial.println("L2 pressed");
    if (ps2x.Button(PSB_R2))
        Serial.println("R2 pressed");

    if (ps2x.Button(PSB_L3))
        Serial.println("L3 pressed");
    if (ps2x.Button(PSB_R3))
        Serial.println("R3 pressed");

    if (ps2x.Button(PSB_L1) || ps2x.Button(PSB_R1))
    { //print stick values if either is TRUE
        Serial.print("Stick Values:");
        Serial.print(ps2x.Analog(PSS_LY), DEC); //Left stick, Y axis. Other options: LX, RY, RX
        Serial.print(",");
        Serial.print(ps2x.Analog(PSS_LX), DEC);
        Serial.print(",");
        Serial.print(ps2x.Analog(PSS_RY), DEC);
        Serial.print(",");
        Serial.println(ps2x.Analog(PSS_RX), DEC);
    }else{
        Serial.print("Stick Values:");
        Serial.print(ps2x.Analog(PSS_LY), DEC); //Left stick, Y axis. Other options: LX, RY, RX
        Serial.print(",");
        Serial.print(ps2x.Analog(PSS_LX), DEC);
        Serial.print(",");
        Serial.print(ps2x.Analog(PSS_RY), DEC);
        Serial.print(",");
        Serial.println(ps2x.Analog(PSS_RX), DEC);
    }
    // RUNCMD = FORWARD;
    Run(RUNCMD);
    // mrun.one(4, leftMotor1, leftMotor2);
    // mrun.two(4,9);
*/

    // 自动关机 start
    // Serial.println(digitalRead(leftMotor1));
    // Serial.println(digitalRead(leftMotor2));
    // Serial.println(digitalRead(rightMotor1));
    // Serial.println(digitalRead(rightMotor2));
    // if (digitalRead(leftMotor1) == LOW && digitalRead(leftMotor2) == LOW && digitalRead(rightMotor1) == LOW && digitalRead(rightMotor2) == LOW)
    if ((RUN_SPEED < 130 && RUN_SPEED > 126) && (LR < 130 && LR > 126))
    {
        if ((millis() - starttime) > 600000)
        { // xx秒没有按，自动关机 10分钟
            go_poweroff();
        }
    }
    else
    {
        starttime = millis();
    }
    // 自动关机 end

    Serial.println();
    delay(10);
}
