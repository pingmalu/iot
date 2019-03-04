/*
    ps2手柄控制4轮驱动小车

 * BY: Malu
 * https://malu.me
 * 2019.2.21

*/

#include <PS2X_lib.h> //for v1.6

#include "MRUN_lib.h"

// // NODEMCU版本引脚
// int leftMotor1 = D5; // 前后轮子
// int leftMotor2 = D6;
// int rightMotor1 = D7; // 左右轮子
// int rightMotor2 = D8;
// // 接收机引脚
// #define PS2_DAT D1 //14
// #define PS2_CMD D2 //15
// #define PS2_CS D3  //16
// #define PS2_CLK D4 //17

// PS2X摇杆
int Y_MAX = 255;
int Y_MID = 127;
int Y_MIN = 0;
int X_MAX = 255;
int X_MID = 128;
int X_MIN = 0;
int SILL = 5; // 偏移阈值

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

// UNO版本引脚
int leftMotor1 = 2; // 左边轮子
int leftMotor2 = 3;
int rightMotor1 = 4; // 右边轮子
int rightMotor2 = 5;
// 接收机引脚
#define PS2_DAT 13 //14
#define PS2_CMD 11 //15
#define PS2_CS 10  //16
#define PS2_CLK 12 //17

/*****************************
 * 
 * 2.4g_tank 引脚
 * 
 // UNO版本引脚
int leftMotor1 = 4; // 左边轮子
int leftMotor2 = 5;
int rightMotor1 = 7; // 右边轮子
int rightMotor2 = 6;
// 接收机引脚
#define PS2_DAT 11 //14
#define PS2_CMD 10 //15
#define PS2_CS  9  //16
#define PS2_CLK 8 //17
 * 
 * **************************/

// 驾驶定义
#define STOP 0
#define FORWARD 1
#define BACKWARD 2
#define TURNLEFT 3
#define TURNRIGHT 4

// 全局驾驶变量
int RUNCMD = 0;
bool TANK_MOD = false;

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

void Run(int cmd)
{
    switch (cmd)
    {
    case FORWARD:
        Serial.print(" FORWARD"); // 前
        mrun.two(MAX_SPEED, MAX_SPEED);
        break;
    case BACKWARD:
        Serial.println("BACKWARD"); // 后
        mrun.two(-MAX_SPEED, -MAX_SPEED);
        break;
    case TURNLEFT:
        Serial.println("TURNLEFT"); // 左
        mrun.two(-MAX_SPEED, MAX_SPEED);
        break;
    case TURNRIGHT:
        Serial.println("TURNRIGHT"); // 右
        mrun.two(MAX_SPEED, -MAX_SPEED);
        break;
    default:
        Serial.print("."); // 停止
        mrun.two();
        break;
        // delay(50);
    }
}

void setup()
{
    Serial.begin(115200);

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
            delay(100);
        }
    } while (1);

    // 电机驱动引脚初始化
    pinMode(leftMotor1, OUTPUT);
    pinMode(leftMotor2, OUTPUT);
    pinMode(rightMotor1, OUTPUT);
    pinMode(rightMotor2, OUTPUT);

    mrun.config(leftMotor1, leftMotor2, rightMotor1, rightMotor2, Y_MAX, Y_MID, Y_MIN, X_MAX, X_MID, X_MIN, SILL);

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
    // 速度初始化
    RUN_SPEED = STOP;
    LR = STOP;

    ps2x.read_gamepad(false, 0); //read controller and set large motor to spin at 'vibrate' speed

    // SELECT 切换坦克模式
    if (ps2x.ButtonReleased(PSB_SELECT))
    {
        Serial.println("PSB_SELECT Button Released!");
        TANK_MOD = TANK_MOD ? false : true;
    }

    if (ps2x.Button(PSB_START)) //will be TRUE as long as button is pressed
        Serial.println("Start is being held");
    if (ps2x.Button(PSB_SELECT))
        Serial.println("Select is being held");

    // 2wd小车
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

    // // 左边按键群
    // if (ps2x.Button(PSB_PAD_UP))
    // { //will be TRUE as long as button is pressed
    //     Serial.println("Up held this hard: ");
    //     RUN_SPEED = Y_MAX;
    // }
    // else if (ps2x.ButtonReleased(PSB_PAD_UP))
    // {
    //     Serial.println("Up Button Released!");
    //     RUN_SPEED = Y_MID;
    // }

    // if (ps2x.Button(PSB_PAD_DOWN))
    // {
    //     Serial.println("DOWN held this hard: ");
    //     RUN_SPEED = Y_MIN;
    // }
    // else if (ps2x.ButtonReleased(PSB_PAD_DOWN))
    // {
    //     Serial.println("DOWN Button Released!");
    //     RUN_SPEED = Y_MID;
    // }

    // if (ps2x.Button(PSB_PAD_RIGHT))
    // {
    //     Serial.println("Right held this hard: ");
    //     LR = X_MAX;
    // }
    // else if (ps2x.ButtonReleased(PSB_PAD_RIGHT))
    // {
    //     Serial.println("Right Button Released!");
    //     LR = X_MID;
    // }

    // if (ps2x.Button(PSB_PAD_LEFT))
    // {
    //     Serial.println("LEFT held this hard: ");
    //     LR = X_MIN;
    // }
    // else if (ps2x.ButtonReleased(PSB_PAD_LEFT))
    // {
    //     Serial.println("LEFT Button Released!");
    //     LR = X_MID;
    // }

    // // 右边按键群
    // if (ps2x.Button(PSB_TRIANGLE))
    // { //will be TRUE as long as button is pressed
    //     Serial.println("Triangle pressed");
    //     RUN_SPEED = Y_MAX;
    // }
    // else if (ps2x.ButtonReleased(PSB_TRIANGLE))
    // {
    //     Serial.println("Triangle Button Released!");
    //     RUN_SPEED = Y_MID;
    // }

    // if (ps2x.Button(PSB_CROSS))
    // {
    //     Serial.println("PSB_CROSS pressed");
    //     RUN_SPEED = Y_MIN;
    // }
    // else if (ps2x.ButtonReleased(PSB_CROSS))
    // {
    //     Serial.println("PSB_CROSS Button Released!");
    //     RUN_SPEED = Y_MID;
    // }

    // if (ps2x.Button(PSB_CIRCLE)) // 右
    // {
    //     Serial.println("PSB_CIRCLE pressed");
    //     LR = X_MAX;
    // }
    // else if (ps2x.ButtonReleased(PSB_CIRCLE))
    // {
    //     Serial.println("PSB_CIRCLE Button Released!");
    //     LR = X_MID;
    // }

    // if (ps2x.Button(PSB_SQUARE)) // 左
    // {
    //     Serial.println("PSB_SQUARE pressed");
    //     LR = X_MIN;
    // }
    // else if (ps2x.ButtonReleased(PSB_SQUARE))
    // {
    //     Serial.println("PSB_SQUARE Button Released!");
    //     LR = X_MID;
    // }

    // Serial.print(" RUN:");
    // mrun.one_l(RUN_SPEED);
    // Serial.print(" LR:");
    // mrun.one_r(LR);

    // Serial.print("Stick Values:");
    // Serial.print(ps2x.Analog(PSS_LY), DEC); //Left stick, Y axis. Other options: LX, RY, RX
    // Serial.print(",");
    // Serial.print(ps2x.Analog(PSS_LX), DEC);
    // Serial.print(",");
    // Serial.print(ps2x.Analog(PSS_RY), DEC);
    // Serial.print(",");
    // Serial.print(ps2x.Analog(PSS_RX), DEC);

    // RUN_SPEED = map(constrain((int)ps2x.Analog(PSS_LY), 0, 255), 0, 255, MAX_SPEED, -MAX_SPEED);

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
            RUN_SPEED = map(constrain((int)ps2x.Analog(PSS_RY), 0, 255), 0, 255, 255, 0);
            LR = map(constrain((int)ps2x.Analog(PSS_RX), 0, 255), 0, 255, 0, 255);
            mrun.tank(RUN_SPEED, LR);
        }
        
    }
    else
    {
        // 坦克坐标变换
        if (LR < 0)
        {
            mrun.two(MAX_SPEED, -MAX_SPEED);
        }
        else if (LR > 0)
        {
            mrun.two(-MAX_SPEED, MAX_SPEED);
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

    Serial.println();
    delay(10);
}
