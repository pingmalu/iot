/**
 * ps3手柄控制4轮驱动小车
 * ps3蓝牙库 https://github.com/jvpernis/esp32-ps3/tree/develop
 * v1 L3键开启AUTO_RUNING_MODE
 *    L1 R1 开灯
 *    R2 加速
 *    L1 R1 L2 R2 start 切换前后
 *    ps3蓝牙控制坦克，机械手臂，LED大灯
 *    PS键显示电量
 *    select键关灯，关闭TANK_V2_MOD，手柄震动
 *    R3键开启TANK_V2_MOD
 * BY: Malu
 * https://malu.me
 * 2020.04.18
 */

#include <Ps3Controller.h>
#include <analogWrite.h>
#include <EEPROM.h>
#include <Servo.h>

// comment out this line, if you want to disable logs:
#define DEBUG

#ifdef DEBUG
#define LOG(...) Serial.print(__VA_ARGS__)
#define LOGLN(...) Serial.println(__VA_ARGS__)
#else
#define LOG(...)
#define LOGLN(...)
#endif

Servo myservo1; // 创建舵机对象

// 前后轮子引脚
int MotorA1 = 2;
int MotorA2 = 4;

// 舵机引脚
#define SERVO_PIN_1 23
#define STOP 0
// analogWrite(pin, value)  UNO:0-255  D1 ESP8266:0-1023
#define MAX_SPEED 1023
#define MID_SPEED 512
#define LOW_SPEED 312
#define START_SPEED 40

int RUN_SPEED = 0; // 推进速度
int LR = 0;        // 转向速度
int LR_MIN = 60;   // 最左值
int LR_MAX = 130;  // 最右值
int EEPROM_0;
int MAX_RUN_SPEED = MID_SPEED;

bool square_tag = false;
bool circle_tag = false;

void setup()
{
    Serial.begin(115200);
    EEPROM.begin(1);
    EEPROM_0 = EEPROM.read(0);
    // 去除pwm啸叫
    analogWriteFrequency(20e6); //更改pwm频率为20kHz，去除啸叫

    Ps3.attach(notify);
    Ps3.attachOnDisconnect(rec);
    Ps3.begin("FF:87:E0:A6:AC:05");

    // 电机驱动引脚初始化
    pinMode(MotorA1, OUTPUT);
    pinMode(MotorA2, OUTPUT);

    // 舵机引脚初始化
    myservo1.attach(SERVO_PIN_1);
}

void loop()
{
}

// 重连
void rec()
{
    LOGLN("*****************down***************");
    Ps3.attach(notify);
    Ps3.begin("FF:87:E0:A6:AC:05");
}

void notify()
{
    // 速度初始化
    RUN_SPEED = STOP;
    LR = STOP;

    // 电池异常停止
    if (Ps3.data.status.battery == 20)
    {
        digitalWrite(MotorA1, LOW);
        analogWrite(MotorA2, LOW, 1023);
        LOGLN("stop");
        return;
    }

    // 调整EEPROM初始值
    if (Ps3.data.button.l1 == 1 && Ps3.data.button.r1 == 1)
    {
        if (Ps3.data.button.square == 1)
        { // 左
            if (!square_tag)
            {
                if (EEPROM_0 > 108)
                {
                    EEPROM_0--;
                }
                EEPROM.write(0, EEPROM_0);
                EEPROM.commit();
            }
            square_tag = true;
        }
        else
        {
            square_tag = false;
        }

        if (Ps3.data.button.circle == 1)
        { // 右
            if (!circle_tag)
            {
                if (EEPROM_0 < 148)
                {
                    EEPROM_0++;
                }
                EEPROM.write(0, EEPROM_0);
                EEPROM.commit();
            }
            circle_tag = true;
        }
        else
        {
            circle_tag = false;
        }

        if (Ps3.data.button.triangle == 1)
        {
            EEPROM_0 = 128;
            EEPROM.write(0, EEPROM_0);
            EEPROM.commit();
        }
    }

    // 加速 减速
    if (Ps3.data.button.r2 == 1 || Ps3.data.button.l2 == 1)
    {
        if (Ps3.data.button.r2 == 1)
        {
            MAX_RUN_SPEED = MAX_SPEED;
        }
        else
        {
            MAX_RUN_SPEED = LOW_SPEED;
        }
    }
    else
    {
        MAX_RUN_SPEED = MID_SPEED;
    }

    if (Ps3.data.button.up == 1 || Ps3.data.button.triangle == 1)
    {
        RUN_SPEED = MAX_RUN_SPEED;
    }
    else if (Ps3.data.button.down == 1 || Ps3.data.button.cross == 1)
    {
        RUN_SPEED = -MAX_RUN_SPEED;
    }

    if (Ps3.data.button.right == 1 || Ps3.data.button.circle == 1)
    {
        LR = -MAX_SPEED;
    }
    else if (Ps3.data.button.left == 1 || Ps3.data.button.square == 1)
    {
        LR = MAX_SPEED;
    }

    if (RUN_SPEED == STOP)
    {
        RUN_SPEED = map(constrain(Ps3.data.analog.stick.ly, -128, 127), -128, 127, MAX_RUN_SPEED, -MAX_RUN_SPEED);
    }
    LOG("RUN:");
    LOG(RUN_SPEED);
    int v_abs = abs(RUN_SPEED);
    v_abs = constrain(v_abs, 0, MAX_RUN_SPEED);
    if (v_abs < START_SPEED) // 小于启动速度，不要发送PWM
    {
        digitalWrite(MotorA1, LOW);
        analogWrite(MotorA2, LOW, 1023);
    }
    else if (RUN_SPEED > STOP)
    {
        digitalWrite(MotorA1, LOW);
        analogWrite(MotorA2, v_abs, 1023);
    }
    else if (RUN_SPEED < STOP)
    {
        digitalWrite(MotorA1, HIGH);
        analogWrite(MotorA2, MAX_SPEED - v_abs, 1023);
    }
    else
    {
        digitalWrite(MotorA1, LOW);
        analogWrite(MotorA2, LOW, 1023);
    }

    if (LR > STOP)
    {
        LR = LR_MIN + EEPROM_0 - 128;
    }
    else if (LR < STOP)
    {
        LR = LR_MAX + EEPROM_0 - 128;
    }
    else
    {
        LR = map(constrain(Ps3.data.analog.stick.rx, -128, 127), -128, 127, LR_MIN + EEPROM_0 - 128, LR_MAX + EEPROM_0 - 128);
    }
    LOG(" LR:");
    LOG(LR);
    myservo1.write(LR);
    LOGLN();
    // delay(10);
    return;
}
