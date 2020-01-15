/*
 * ps3手柄控制4轮驱动小车
 * ps3蓝牙库 https://github.com/jvpernis/esp32-ps3/tree/develop
 * v2 L3键开启AUTO_RUNING_MODE
 *    L1 R1 开灯
 *    R2 加速
 *    L1 R1 L2 R2 start 切换前后
 * v1 ps3蓝牙控制坦克，机械手臂，LED大灯
 *    PS键显示电量
 *    select键关灯，关闭TANK_V2_MOD，手柄震动
 *    R3键开启TANK_V2_MOD
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
// #include <Servo.h>
#include <analogWrite.h>
#include <EEPROM.h>

// comment out this line, if you want to show logs:
#define NDEBUG

#ifdef NDEBUG
#define LOG(...)
#define LOGLN(...)
#else
#define LOG(...) Serial.print(__VA_ARGS__)
#define LOGLN(...) Serial.println(__VA_ARGS__)
#endif

// 超声波针脚定义
int EchoPin = 14; // 收到反射回来的超声波
int TrigPin = 13; // 发出超声波

int LED_PIN1 = 5;
int LED_PIN2 = 22;

// Servo myservolno;
// Servo myservorno;
// Servo myservo1;
// Servo myservo2;
// Servo myservo3;
// Servo myservo4;
// Servo myservoledno;
// 舵机引脚
int SPIN_1 = 23;
int SPIN_2 = 18;
int SPIN_3 = 19;
int SPIN_4 = 21;

// 电机驱动引脚
int leftMotor1 = 2; // 前后轮子
int leftMotor2 = 4;
int rightMotor1 = 16; // 左右轮子
int rightMotor2 = 17;

// 舵机初始角度
int16_t SVO1 = 90;
int16_t SVO2 = 90;
int16_t SVO3 = 90;
int16_t SVO4 = 90;

// PSx摇杆
int Y_MAX = 255;
int Y_MID = 127;
int Y_MIN = 0;
int X_MAX = 255;
int X_MID = 127;
int X_MIN = 0;
int SILL = 2; // 偏移阈值

// 驾驶定义
#define STOP 0

// 全局驾驶变量
bool LED_MOD = false;
bool TANK_MOD = true;
bool TANK_V2_MOD = true;

MRUN mrun;

// analogWrite(pin, value)  UNO:0-255  D1 ESP8266:0-1023
#define MAX_SPEED 1023

int RUN_SPEED = 0; // 推进速度
int LR = 0;        // 转向速度

#define SPEED_MAX 1023
#define SPEED_MID 512
#define SPEED_MIN 400

ps3_cmd_t cmd;

// 按键历史值
bool BTN_R1 = false;
bool BTN_R1_TAG = false; // LED_PIN1开关
bool BTN_L1 = false;
bool BTN_L1_TAG = false; // LED_PIN2开关
bool BTN_L3 = false;
bool BTN_L3_TAG = false; // 自动运行开关
bool BTN_UPDWON = false;

// EEPROM缓冲值
bool EEP_UP_DWON_TAG = false; // 前后切换

// 最大速度缓存值
uint16_t MAX_SPEED_INIT = 0;

// 自动前进初始时间
unsigned long starttime;
bool AUTO_RUNING_MODE = false;
float AUTO_RUNING_DISTANCE = 0.0f;

// define task
TaskHandle_t Task1;

int pr(int16_t val)
{
    val = map(val, -128, 127, 0, 255);
    LOG(" [");
    LOG(val);
    LOG("]");
    return val;
}

int16_t pr_f(int16_t val)
{
    val = map(val, -128, 127, 255, 0);
    LOG(" [");
    LOG(val);
    LOG("]");
    return val;
}

int16_t pr180(int16_t val)
{
    val = map(val, -128, 127, 0, 180);
    LOG(" (");
    LOG(val);
    LOG(")");
    return val;
}

// 手柄震动
void shock()
{
    LOG("shocking");
    LOG(" ");
    // cmd.led1 = true;
    cmd.rumble_left_intensity = 0xff; //强度
    cmd.rumble_right_intensity = 0xff;
    cmd.rumble_right_duration = 100; //持续时间
    cmd.rumble_left_duration = 100;
    ps3Cmd(cmd);
}

// 默认灯亮效果
void defled()
{
    Ps3.setLed(1);
}

// 手柄电量显示v2
void battery_info_v2(int i)
{
    LOG("battery:");
    LOG(i);
    LOG(" ");
    switch (i)
    {
    case 1:
        cmd.led1 = true;
        cmd.led2 = false;
        cmd.led3 = false;
        cmd.led4 = false;
        break;
    case 2:
        cmd.led1 = true;
        cmd.led2 = true;
        cmd.led3 = false;
        cmd.led4 = false;
        break;
    case 3:
        cmd.led1 = true;
        cmd.led2 = true;
        cmd.led3 = true;
        cmd.led4 = false;
        break;
    case 4:
        cmd.led1 = true;
        cmd.led2 = true;
        cmd.led3 = true;
        cmd.led4 = true;
        break;
    case 5:
        cmd.led1 = false;
        cmd.led2 = true;
        cmd.led3 = true;
        cmd.led4 = true;
        break;
    default:
        cmd.led1 = true;
        cmd.led2 = false;
        cmd.led3 = false;
        cmd.led4 = true;
        break;
    }
    ps3Cmd(cmd);
}

/**
 * 获取超声波距离
 **/
int getDistance()
{
    digitalWrite(TrigPin, LOW); // 使发出发出超声波信号接口低电平2μs
    delayMicroseconds(2);
    digitalWrite(TrigPin, HIGH); // 使发出发出超声波信号接口高电平10μs，这里是至少10μs
    delayMicroseconds(10);
    digitalWrite(TrigPin, LOW); // 保持发出超声波信号接口低电平

    float distance;

    /*
  distance = pulseIn(EchoPin, HIGH) / 58.0; //将回波时间换算成cm 
  distance = (int(distance * 100.0)) / 100.0; //保留两位小数 
  */

    distance = pulseIn(EchoPin, HIGH); //读出脉冲时间
    distance = distance * 0.017;       //脉冲时间转化为距离值

    LOG(distance); //输出距离值
    LOGLN('cm');

    return distance;
}

void start_distance(void *parameter)
{
    Serial.print("start_distance() running on core ");
    Serial.println(xPortGetCoreID());
    while (1)
    {
        AUTO_RUNING_DISTANCE = getDistance();
        Serial.print(AUTO_RUNING_DISTANCE);
        Serial.println();
        delay(100);
    }
}

void stop_subprocess()
{
    Serial.print("stop_sub ");
    vTaskDelete(Task1);
}

void start_subprocess()
{
    Serial.print("start_sub ");
    xTaskCreatePinnedToCore(
        start_distance, /* Function to implement the task */
        "Task1",        /* Name of the task */
        10000,          /* Stack size in words */
        NULL,           /* Task input parameter */
        2,              /* Priority of the task */
        &Task1,         /* Task handle. */
        1);             /* Core where the task should run */
}

/**
 * 自动探测
 */
void auto_run()
{
    if (AUTO_RUNING_DISTANCE > 30)
    {
        if (AUTO_RUNING_DISTANCE > 50)
        {
            if (AUTO_RUNING_DISTANCE > 100)
            {
                mrun.two(300, 300);
            }
            else
            {
                mrun.two(250, 250);
            }
        }
        else
        {
            mrun.two(200, 200);
        }
    }
    else
    {
        mrun.two(250, -250);
        // if (!AUTO_RUNING_MODE)
        // {
        //     mrun.two(MAX_SPEED, -MAX_SPEED);
        //     AUTO_RUNING_MODE = true;
        //     starttime = millis();
        // }
        // else
        // {
        //     if ((millis() - starttime) > 1000)
        //     { // 1秒后
        //         // 停止
        //         mrun.two(STOP, STOP);
        //     }
        // }
    }
}

void notify()
{
    // LOG(Ps3.data.status.battery);
    // LOG(" ");
    // LOG(Ps3.data.analog.stick.ry);
    // LOG(" ");
    // 防止信号中断自动前进
    // 4 -1  [128] [128] [128] [128] tank_v2_1:128 tank_v2_2:128 two_L:0 tow_R:0
    // 20 -128  [255] [128] tank_L:255 tank_R:128 one:1023 one:1023 two_L:1023 tow_R:1023
    if (Ps3.data.status.battery == 20)
    {
        mrun.two(STOP, STOP);
        LOG("stop");
        LOGLN();
        return;
    }

    // 切换前后
    if (Ps3.data.button.l1 == 1 && Ps3.data.button.r1 == 1 && Ps3.data.button.l2 == 1 && Ps3.data.button.r2 == 1 && Ps3.data.button.start == 1)
    {
        if (!BTN_UPDWON)
        {
            EEP_UP_DWON_TAG = EEP_UP_DWON_TAG ? false : true;
            mrun.EEP_UP_DWON_TAG = EEP_UP_DWON_TAG;
            EEPROM.write(0, EEP_UP_DWON_TAG ? 1 : 0); // 0:false  1:true
            EEPROM.commit();
        }
        BTN_UPDWON = true;
    }
    else
    {
        BTN_UPDWON = false;
    }

    //显示手柄电量
    if (Ps3.data.button.ps == 1)
    {
        Serial.print("notify() running on core ");
        Serial.println(xPortGetCoreID());
        battery_info_v2(Ps3.data.status.battery);
        LOGLN();
        MAX_SPEED_INIT = SPEED_MAX;
        mrun.MAX_RUN_SPEED = MAX_SPEED_INIT;
        return;
    }

    if (Ps3.data.button.select == 1)
    {
        // LED_MOD = false;
        // TANK_V2_MOD = false;
        // shock(); //震动
        // LOGLN();
        defled();
        MAX_SPEED_INIT = SPEED_MID;
        mrun.MAX_RUN_SPEED = MAX_SPEED_INIT;
        return;
    }

    // 继电器通断
    if (Ps3.data.button.r1 == 1)
    {
        // 上一次为释放时才进去
        if (!BTN_R1)
        {
            // 切换开关
            BTN_R1_TAG = BTN_R1_TAG ? false : true;
            if (BTN_R1_TAG)
            {
                digitalWrite(LED_PIN1, HIGH);
            }
            else
            {
                digitalWrite(LED_PIN1, LOW);
            }
        }
        BTN_R1 = true;
    }
    else
    {
        BTN_R1 = false;
    }

    // 继电器通断
    if (Ps3.data.button.l1 == 1)
    {
        // 上一次为释放时才进去
        if (!BTN_L1)
        {
            // 切换开关
            BTN_L1_TAG = BTN_L1_TAG ? false : true;
            if (BTN_L1_TAG)
            {
                digitalWrite(LED_PIN2, HIGH);
            }
            else
            {
                digitalWrite(LED_PIN2, LOW);
            }
        }
        BTN_L1 = true;
    }
    else
    {
        BTN_L1 = false;
    }

    // 速度调至最低
    if (Ps3.data.button.start == 1)
    {
        battery_info_v2(2);
        MAX_SPEED_INIT = SPEED_MIN;
        mrun.MAX_RUN_SPEED = MAX_SPEED_INIT;
    }

    // r2变速
    if (Ps3.data.analog.button.r2 > 0)
    {
        LOG(Ps3.data.analog.button.r2);
        LOGLN();
        uint16_t SPEED_TMP1 = MAX_SPEED_INIT + (Ps3.data.analog.button.r2 * 3);
        SPEED_TMP1 = (SPEED_TMP1 >= 1023) ? 1023 : SPEED_TMP1;
        mrun.MAX_RUN_SPEED = SPEED_TMP1;
    }
    else
    {
        mrun.MAX_RUN_SPEED = MAX_SPEED_INIT;
    }

    // 自动超声波避障
    if (Ps3.data.button.l3 == 1)
    {
        // 上一次为释放时才进去
        if (!BTN_L3)
        {
            // 切换开关
            // BTN_L3_TAG = BTN_L3_TAG ? false : true;
            AUTO_RUNING_MODE = AUTO_RUNING_MODE ? false : true;
            if (AUTO_RUNING_MODE)
            {
                start_subprocess();
            }
            else
            {
                stop_subprocess();
            }
        }
        BTN_L3 = true;
    }
    else
    {
        BTN_L3 = false;
    }

    // // 舵机
    // if (Ps3.data.button.r1 == 1)
    // {
    //     SVO2 = 165;
    // }
    // else
    // {
    //     SVO2 = 100;
    // }

    // if (Ps3.data.button.l1 == 1)
    // {
    //     SVO1 = pr180(Ps3.data.analog.stick.lx);
    //     SVO3 = pr180(Ps3.data.analog.stick.ly);
    // }
    // else
    // {
    //     if (Ps3.data.analog.stick.lx > 50)
    //     {
    //         if (SVO1 < 180)
    //             SVO1++;
    //     }
    //     if (Ps3.data.analog.stick.lx < -50)
    //     {
    //         if (SVO1 > 0)
    //             SVO1--;
    //     }

    //     if (Ps3.data.analog.stick.ly > 50)
    //     {
    //         if (SVO3 < 99)
    //             SVO3++;
    //     }
    //     if (Ps3.data.analog.stick.ly < -50)
    //     {
    //         if (SVO3 > 0)
    //             SVO3--;
    //     }
    // }

    // if (Ps3.data.button.r2 == 1)
    // {
    //     if (SVO4 < 130)
    //         SVO4++;
    // }
    // if (Ps3.data.button.l2 == 1)
    // {
    //     if (SVO4 > 45)
    //         SVO4--;
    // }
    // myservo1.write(SVO1);
    // myservo2.write(SVO2);
    // myservo3.write(SVO3);
    // myservo4.write(SVO4);
    // LOG("SVO1:");
    // LOG(SVO1);
    // LOG(" ");
    // LOG("SVO2:");
    // LOG(SVO2);
    // LOG(" ");
    // LOG("SVO3:");
    // LOG(SVO3);
    // LOG(" ");
    // LOG("SVO4:");
    // LOG(SVO4);
    // LOG(" ");

    // 速度初始化
    RUN_SPEED = STOP;
    LR = STOP;

    // 按键群
    if (Ps3.data.button.up == 1 || Ps3.data.button.triangle == 1)
    {
        RUN_SPEED = EEP_UP_DWON_TAG ? MAX_SPEED : -MAX_SPEED;
    }
    else if (Ps3.data.button.down == 1 || Ps3.data.button.cross == 1)
    {
        RUN_SPEED = EEP_UP_DWON_TAG ? -MAX_SPEED : MAX_SPEED;
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

    // L3开启自动前进
    if (RUN_SPEED != STOP || LR != STOP)
    {
        // 左右上下按下时退出自动前进
        // BTN_L3_TAG = false;
        if (AUTO_RUNING_MODE)
        {
            AUTO_RUNING_MODE = false;
            stop_subprocess();
        }
    }
    if (AUTO_RUNING_MODE)
    {
        auto_run();
        LOGLN();
        return;
    }

    if (RUN_SPEED == STOP && LR == STOP) // 在按键全部释放
    {
        if (EEP_UP_DWON_TAG)
        {
            RUN_SPEED = pr_f(Ps3.data.analog.stick.ly);
            LR = pr_f(Ps3.data.analog.stick.rx);
        }
        else
        {
            RUN_SPEED = pr(Ps3.data.analog.stick.ly);
            LR = pr(Ps3.data.analog.stick.rx);
        }
        if (RUN_SPEED == Y_MID && LR == X_MID) // 右摇杆不在控制
        {
            mrun.two(STOP, STOP);
        }
        else
        {
            mrun.tank(RUN_SPEED, LR);
        }
        // // 右摇杆
        // RUN_SPEED = pr(Ps3.data.analog.stick.ry);
        // LR = pr(Ps3.data.analog.stick.rx);

        // if (RUN_SPEED == Y_MID && LR == X_MID) // 右摇杆不在控制
        // {
        //     // 左摇杆
        //     RUN_SPEED = pr(Ps3.data.analog.stick.ly);
        //     LR = pr(Ps3.data.analog.stick.lx);
        //     if (RUN_SPEED == Y_MID && LR == X_MID) // 右摇杆不在控制
        //     {
        //         mrun.two(STOP, STOP);
        //     }
        //     else
        //     {
        //         mrun.tank(RUN_SPEED, LR);
        //     }

        //     // maluservo(pr180(Ps3.data.analog.stick.lx), SPIN_1);
        //     // delay(1);
        //     // myservo3.write(pr180(Ps3.data.analog.stick.lx));
        //     // mrun.two(STOP, STOP);
        // }
        // else
        // {
        //     mrun.tank(RUN_SPEED, LR);
        // }
    }
    else
    {
        // 坦克坐标变换
        if (LR < 0)
        {
            if (RUN_SPEED > STOP)
            {
                // mrun.two(MAX_SPEED, STOP);
                mrun.two(MAX_SPEED, STOP + MAX_SPEED_INIT - SPEED_MIN);
            }
            else if (RUN_SPEED < STOP)
            {
                // mrun.two(STOP, -MAX_SPEED);
                mrun.two(STOP - MAX_SPEED_INIT + SPEED_MIN, -MAX_SPEED);
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
                // mrun.two(STOP, MAX_SPEED);
                mrun.two(STOP + MAX_SPEED_INIT - SPEED_MIN, MAX_SPEED);
            }
            else if (RUN_SPEED < STOP)
            {
                // mrun.two(-MAX_SPEED, STOP);
                mrun.two(-MAX_SPEED, STOP - MAX_SPEED_INIT + SPEED_MIN);
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
    LOGLN();
    // delay(10);
}

// 重连
void rec()
{
    LOG("*****************down***************");
    LOGLN();
    Ps3.attach(notify);
    Ps3.begin("FF:87:E0:A6:AC:05");
}

void setup()
{
    Serial.begin(115200);

    // myservolno.attach(SPIN_1); //电机驱动pwm通道占位
    // myservorno.attach(SPIN_1); //电机驱动pwm通道占位
    // myservo1.attach(SPIN_1);
    // myservo2.attach(SPIN_2);
    // myservo3.attach(SPIN_3);
    // myservo4.attach(SPIN_4);
    // myservoledno.attach(SPIN_1); //LED电机驱动pwm通道占位
    // 超声波控制引脚初始化
    pinMode(EchoPin, INPUT);
    pinMode(TrigPin, OUTPUT);

    pinMode(LED_PIN1, OUTPUT);
    pinMode(LED_PIN2, OUTPUT);

    Ps3.attach(notify);
    Ps3.attachOnDisconnect(rec);
    Ps3.begin("FF:87:E0:A6:AC:05");

    // 电机驱动引脚初始化
    pinMode(leftMotor1, OUTPUT);
    pinMode(leftMotor2, OUTPUT);
    pinMode(rightMotor1, OUTPUT);
    pinMode(rightMotor2, OUTPUT);

    //获取前后切换值
    EEPROM.begin(1); // 使用EEPROM时，首先调用EEPROM.begin(size)，size为需要读写的数据字节最大地址+1，取值1~4096
    EEP_UP_DWON_TAG = EEPROM.read(0) == 0 ? false : true;

    mrun.config(leftMotor1, leftMotor2, rightMotor1, rightMotor2, Y_MAX, Y_MID, Y_MIN, X_MAX, X_MID, X_MIN, SILL);
    mrun.EEP_UP_DWON_TAG = EEP_UP_DWON_TAG;

    // 初始最大速度
    MAX_SPEED_INIT = SPEED_MID;
    mrun.MAX_RUN_SPEED = MAX_SPEED_INIT;
}

void loop()
{
}
