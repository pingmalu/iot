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
#include <Servo.h>
Servo myservolno;
Servo myservorno;
Servo myservo1;
Servo myservo2;
Servo myservo3;
Servo myservo4;
// 舵机引脚
int SPIN_1 = 5;
int SPIN_2 = 18;
int SPIN_3 = 19;
int SPIN_4 = 21;
int16_t SVO1 = 90;
int16_t SVO2 = 90;
int16_t SVO3 = 90;
int16_t SVO4 = 90;

// esp32-cam引脚
int leftMotor1 = 2; // 前后轮子
int leftMotor2 = 4;
int rightMotor1 = 16; // 左右轮子
int rightMotor2 = 17;

// PSx摇杆
int Y_MAX = 255;
int Y_MID = 128;
int Y_MIN = 0;
int X_MAX = 255;
int X_MID = 128;
int X_MIN = 0;
int SILL = 2; // 偏移阈值

// 驾驶定义
#define STOP 0

// 全局驾驶变量
bool TANK_MOD = true;
bool TANK_V2_MOD = true;
bool LED_MOD = false;
int LED_MOD_NUM = 0;

MRUN mrun;

// analogWrite(pin, value)  UNO:0-255  D1 ESP8266:0-1023
#define MAX_SPEED 1023

int RUN_SPEED = 0; // 推进速度
int LR = 0;        // 转向速度

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

int16_t pr180(int16_t val)
{
    val = map(val, -128, 127, 0, 180);
    Serial.print(" (");
    Serial.print(val);
    Serial.print(")");
    return val;
}

void notify()
{
    Ps3.setLed(2);
    Serial.print(Ps3.data.status.battery);
    Serial.print(" ");
    Serial.print(Ps3.data.analog.stick.ry);
    Serial.print(" ");
    // 防止信号中断自动前进
    // 4 -1  [128] [128] [128] [128] tank_v2_1:128 tank_v2_2:128 two_L:0 tow_R:0
    // 20 -128  [255] [128] tank_L:255 tank_R:128 one:1023 one:1023 two_L:1023 tow_R:1023
    if (Ps3.data.status.battery == 20)
    {
        mrun.two(STOP, STOP);
        Serial.print("stop");
        Serial.println();
        return;
    }

    // 手柄震动
    if (Ps3.data.button.start == 1)
    {
        // ps3_cmd_t cmd;

        // cmd.led1 = true;

        // cmd.rumble_left_intensity = 0xff;
        // cmd.rumble_right_intensity = 0xff;

        // cmd.rumble_right_duration = 100;
        // cmd.rumble_left_duration = 100;

        // ps3Cmd(cmd);
        LED_MOD = true;
    }

    if (Ps3.data.button.r3 == 1)
    {
        TANK_V2_MOD = true;
    }

    if (Ps3.data.button.select == 1)
    {

        LED_MOD = false;
        TANK_V2_MOD = false;
    }

    if (LED_MOD)
    {
        digitalWrite(23, LOW);
        digitalWrite(22, HIGH);
    }
    else
    {
        digitalWrite(22, LOW);
        digitalWrite(23, HIGH);
    }

    // 舵机2
    if (Ps3.data.button.r1 == 1)
    {
        SVO2 = 160;
    }
    else
    {
        SVO2 = 100;
    }
    myservo2.write(SVO2);
    Serial.print(SVO2);
    Serial.print(" ");

    if (Ps3.data.button.l1 == 1)
    {
        myservo1.write(pr180(Ps3.data.analog.stick.lx));
        myservo3.write(pr180(Ps3.data.analog.stick.ly));
    }
    else
    {
        if (Ps3.data.analog.stick.lx > 50)
        {
            if (SVO1 < 180)
                SVO1++;
        }
        if (Ps3.data.analog.stick.lx < -50)
        {
            if (SVO1 > 0)
                SVO1--;
        }

        if (Ps3.data.analog.stick.ly > 50)
        {
            if (SVO3 < 99)
                SVO3++;
        }
        if (Ps3.data.analog.stick.ly < -50)
        {
            if (SVO3 > 0)
                SVO3--;
        }
        myservo1.write(SVO1);
        myservo3.write(SVO3);
        Serial.print(SVO1);
        Serial.print(" ");
        Serial.print(SVO3);
        Serial.print(" ");
    }

    if (Ps3.data.button.r2 == 1)
    {
        if (SVO4 < 130)
            SVO4++;
    }
    if (Ps3.data.button.l2 == 1)
    {
        if (SVO4 > 45)
            SVO4--;
    }
    // myservo2.write(SVO2);
    myservo4.write(SVO4);
    Serial.print(SVO4);
    Serial.print(" ");
        // myservo1.write(pr180(Ps3.data.analog.stick.lx));
        // // // Serial.print(Ps3.data.analog.button.r2);
        // if (Ps3.data.button.r2 == 1)
        // {
        //     myservo4.write(pr180(Ps3.data.analog.stick.ly));
        // }
        // else
        // {
        //     myservo3.write(pr180(Ps3.data.analog.stick.ly));
        // }

        // 速度初始化
        RUN_SPEED = STOP;
    LR = STOP;

    // 按键群
    if (Ps3.data.button.up == 1 || Ps3.data.button.triangle == 1)
    {
        RUN_SPEED = -MAX_SPEED;
    }
    else if (Ps3.data.button.down == 1 || Ps3.data.button.cross == 1)
    {
        RUN_SPEED = MAX_SPEED;
    }
    else
    {
        RUN_SPEED = STOP;
    }

    if (Ps3.data.button.right == 1 || Ps3.data.button.circle == 1)
    {
        LR = MAX_SPEED;
    }
    else if (Ps3.data.button.left == 1 || Ps3.data.button.square == 1)
    {
        LR = -MAX_SPEED;
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

        if (RUN_SPEED == 128 && LR == 128) // 右摇杆不在控制
        {
            // 左摇杆
            // RUN_SPEED = pr_f(Ps3.data.analog.stick.ly);
            // LR = pr_f(Ps3.data.analog.stick.lx);
            // mrun.tank_v2(RUN_SPEED, LR);

            // maluservo(pr180(Ps3.data.analog.stick.lx), SPIN_1);
            // delay(1);
            // myservo3.write(pr180(Ps3.data.analog.stick.lx));
            mrun.two(STOP, STOP);
        }
        else
        {
            if (TANK_V2_MOD)
            {
                mrun.tank_v2(RUN_SPEED, LR);
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
    Serial.println();
    // delay(10);
}

// 重连
void rec()
{
    Serial.print("*****************down***************");
    Serial.println();
    Ps3.attach(notify);
    Ps3.begin("FF:87:E0:A6:AC:05");
}

void setup()
{
    Serial.begin(115200);

    myservolno.attach(SPIN_1);
    myservorno.attach(SPIN_1);
    myservo1.attach(SPIN_1);
    myservo2.attach(SPIN_2);
    myservo3.attach(SPIN_3);
    myservo4.attach(SPIN_4);

    pinMode(23, OUTPUT);
    pinMode(22, OUTPUT);

    Ps3.attach(notify);
    Ps3.attachOnDisconnect(rec);
    Ps3.begin("FF:87:E0:A6:AC:05");

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
