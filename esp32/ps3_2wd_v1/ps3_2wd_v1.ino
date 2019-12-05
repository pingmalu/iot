/*
 * ps3手柄控制4轮驱动小车
 * ps3蓝牙库 https://github.com/jvpernis/esp32-ps3/tree/develop
 * old
 * 4.2版本加入舵机摇杆控制
 * 4.3版本加入坦克模式切换蜂鸣模拟
 * 5.0版本修改MRUN库，增加最大速度控制
 * 5.1版本专门适配迷你四轮驱动小车n20电机驱动
 * 5.2版本专门适配mg996舵机四轮驱动大车，推土机
 * BY: Malu
 * https://malu.me
 * 2019.12.03
 */

#include "MRUN_lib.h"
#include <Ps3Controller.h>

// esp32-cam引脚
int leftMotor1 = 12; // 前后轮子
int leftMotor2 = 13;
int rightMotor1 = 14; // 左右轮子
int rightMotor2 = 15;

// PSx摇杆
int Y_MAX = 255;
int Y_MID = 127;
int Y_MIN = 0;
int X_MAX = 255;
int X_MID = 127;
int X_MIN = 0;
int SILL = 6; // 偏移阈值

// 舵机角度值
int lx;
// int ly;

// 驾驶定义
#define STOP 0

// 全局驾驶变量
bool TANK_MOD = true;

MRUN mrun;

// byte type = 0;

// analogWrite(pin, value)  UNO:0-255  D1 ESP8266:0-1023
#define MAX_SPEED 1023

int RUN_SPEED = 0; // 推进速度
int LR = 0;        // 转向速度
// int START_TAG = 0;

int pr(int16_t val)
{
    val = map(val, -128, 127, 0, 255);
    Serial.print(" [");
    Serial.print(val);
    Serial.print("]");
    return val;
}

int16_t pr_f(int16_t val)
{
    val = map(val, -128, 127, 255, 0);
    Serial.print(" [");
    Serial.print(val);
    Serial.print("]");
    return val;
}

void notify()
{
    // 速度初始化
    RUN_SPEED = STOP;
    LR = STOP;

    // 按键群
    if (Ps3.data.button.up == 1 || Ps3.data.button.triangle == 1)
    {
        RUN_SPEED = MAX_SPEED;
    }
    else if (Ps3.data.button.down == 1 || Ps3.data.button.cross == 1)
    {
        RUN_SPEED = -MAX_SPEED;
    }
    else
    {
        RUN_SPEED = STOP;
    }

    if (Ps3.data.button.right == 1 || Ps3.data.button.circle == 1)
    {
        LR = -MAX_SPEED;
    }
    else if (Ps3.data.button.left == 1 || Ps3.data.button.square == 1)
    {
        LR = MAX_SPEED;
    }
    else
    {
        LR = STOP;
    }

    if (RUN_SPEED == STOP && LR == STOP) // 在按键全部释放
    {
        // 右摇杆
        RUN_SPEED = pr(Ps3.data.analog.stick.ry);
        LR = pr(Ps3.data.analog.stick.rx);

        if (RUN_SPEED == 127 && LR == 127) // 右摇杆不在控制
        {
            // 左摇杆
            RUN_SPEED = pr(Ps3.data.analog.stick.ly);
            LR = pr(Ps3.data.analog.stick.lx);
            mrun.tank_v2(RUN_SPEED, LR);
        }
        else
        {
            mrun.tank(RUN_SPEED, LR);
        }
    }
    else
    {
        // 坦克坐标变换
        if (LR < STOP)
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
        else if (LR > STOP)
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
    Serial.print("\n");
}

void setup()
{
    Serial.begin(115200);
    Ps3.attach(notify);
    Ps3.begin("00:1a:7d:da:71:13");

    // 电机驱动引脚初始化
    pinMode(leftMotor1, OUTPUT);
    pinMode(leftMotor2, OUTPUT);
    pinMode(rightMotor1, OUTPUT);
    pinMode(rightMotor2, OUTPUT);

    mrun.config(leftMotor1, leftMotor2, rightMotor1, rightMotor2, Y_MAX, Y_MID, Y_MIN, X_MAX, X_MID, X_MIN, SILL);

    // 初始最大速度
    mrun.MAX_RUN_SPEED = 1023;
}

void loop()
{
}
