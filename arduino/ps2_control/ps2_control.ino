/*
    ps2手柄控制4轮驱动小车

 * BY: Malu
 * https://malu.me
 * 2019.2.21

*/

#include <PS2X_lib.h> //for v1.6

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
int up = 2; // 左边轮子
int down = 3;
int left = 4; // 右边轮子
int right = 5;
// 接收机引脚
#define PS2_DAT 13 //14
#define PS2_CMD 12 //15
#define PS2_CS 11  //16
#define PS2_CLK 10 //17

/******************************************************************
   select modes of PS2 controller:
     - pressures = analog reading of push-butttons
     - rumble    = motor rumbling
   uncomment 1 of the lines for each mode selection
 ******************************************************************/
#define pressures false
#define rumble false

PS2X ps2x; // create PS2 Controller Class

// 驾驶定义
#define STOP 0
#define FORWARD 1
#define BACKWARD 2
#define TURNLEFT 3
#define TURNRIGHT 4

// 全局驾驶变量
int RUNCMD = 0;

void Run(int cmd)
{
    switch (cmd)
    {
    case FORWARD:
        Serial.print("FORWARD"); // 前
        digitalWrite(up, LOW);
        break;
    case BACKWARD:
        Serial.println("BACKWARD"); // 后
        digitalWrite(down, LOW);
        break;
    case TURNLEFT:
        Serial.println("TURNLEFT"); // 左
        digitalWrite(left, LOW);
        break;
    case TURNRIGHT:
        Serial.println("TURNRIGHT"); // 右
        digitalWrite(right, LOW);
        break;
    default:
        Serial.print("."); // 停止
        digitalWrite(up, HIGH);
        digitalWrite(down, HIGH);
        digitalWrite(left, HIGH);
        digitalWrite(right, HIGH);
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
    pinMode(up, OUTPUT);
    pinMode(down, OUTPUT);
    pinMode(left, OUTPUT);
    pinMode(right, OUTPUT);

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

    ps2x.read_gamepad(false, 0); //read controller and set large motor to spin at 'vibrate' speed

    if (ps2x.Button(PSB_PAD_UP))
    { //will be TRUE as long as button is pressed
        Serial.println("Up held this hard: ");
        RUNCMD = FORWARD;
        digitalWrite(up, LOW);
    }
    else if (ps2x.ButtonReleased(PSB_PAD_UP))
    {
        Serial.println("Up Button Released!");
        RUNCMD = STOP;
        digitalWrite(up, HIGH);
    }

    if (ps2x.Button(PSB_PAD_DOWN))
    {
        Serial.println("DOWN held this hard: ");
        RUNCMD = BACKWARD;
        digitalWrite(down, LOW);
    }
    else if (ps2x.ButtonReleased(PSB_PAD_DOWN))
    {
        Serial.println("DOWN Button Released!");
        RUNCMD = STOP;
        digitalWrite(down, HIGH);
    }

    if (ps2x.Button(PSB_PAD_RIGHT))
    {
        Serial.println("Right held this hard: ");
        RUNCMD = TURNRIGHT;
        digitalWrite(right, LOW);
    }
    else if (ps2x.ButtonReleased(PSB_PAD_RIGHT))
    {
        Serial.println("Right Button Released!");
        RUNCMD = STOP;
        digitalWrite(right, HIGH);
    }

    if (ps2x.Button(PSB_PAD_LEFT))
    {
        Serial.println("LEFT held this hard: ");
        RUNCMD = TURNLEFT;
        digitalWrite(left, LOW);
    }
    else if (ps2x.ButtonReleased(PSB_PAD_LEFT))
    {
        Serial.println("LEFT Button Released!");
        RUNCMD = STOP;
        digitalWrite(left, HIGH);
    }

    // 右边按键群
    if (ps2x.Button(PSB_TRIANGLE))
    { //will be TRUE as long as button is pressed
        Serial.println("Triangle pressed");
        RUNCMD = FORWARD;
        digitalWrite(up, LOW);
    }
    else if (ps2x.ButtonReleased(PSB_TRIANGLE))
    {
        Serial.println("Triangle Button Released!");
        RUNCMD = STOP;
        digitalWrite(up, HIGH);
    }

    if (ps2x.Button(PSB_CROSS))
    {
        Serial.println("PSB_CROSS pressed");
        RUNCMD = BACKWARD;
        digitalWrite(down, LOW);
    }
    else if (ps2x.ButtonReleased(PSB_CROSS))
    {
        Serial.println("PSB_CROSS Button Released!");
        RUNCMD = STOP;
        digitalWrite(down, HIGH);
    }

    if (ps2x.Button(PSB_CIRCLE)) // 右
    {
        Serial.println("PSB_CIRCLE pressed");
        RUNCMD = TURNRIGHT;
        digitalWrite(right, LOW);
    }
    else if (ps2x.ButtonReleased(PSB_CIRCLE))
    {
        Serial.println("PSB_CIRCLE Button Released!");
        RUNCMD = STOP;
        digitalWrite(right, HIGH);
    }

    if (ps2x.Button(PSB_SQUARE)) // 左
    {
        Serial.println("PSB_SQUARE pressed");
        RUNCMD = TURNLEFT;
        digitalWrite(left, LOW);
    }
    else if (ps2x.ButtonReleased(PSB_SQUARE))
    {
        Serial.println("PSB_SQUARE Button Released!");
        digitalWrite(left, HIGH);
        RUNCMD = STOP;
    }

    // Run(RUNCMD);

    Serial.println();
    delay(10);
}
