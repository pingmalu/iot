/**
 * ps2手柄控制4轮驱动小车
 * 4.2版本加入舵机摇杆控制
 * 4.3版本加入坦克模式切换蜂鸣模拟
 * 5.0版本修改MRUN库，增加最大速度控制
 * 5.1版本专门适配迷你四轮驱动小车n20电机驱动
 * 5.2版本专门适配mg996舵机四轮驱动大车，推土机
 * ps2_L298N_esp8266版本重新优化5.2，修复断联问题，去除pwm啸叫，专门适配esp8266
 * ps2_L298N_esp8266_car只适用于car舵机版
 * BY: Malu
 * https://malu.me
 * 2020.02.07
 */

#include <PS2X_lib.h> //for v1.6
#include <Servo.h>    // 舵机控制

// comment out this line, if you want to disable logs:
// #define DEBUG

#ifdef DEBUG
#define LOG(...) Serial.print(__VA_ARGS__)
#define LOGLN(...) Serial.println(__VA_ARGS__)
#else
#define LOG(...)
#define LOGLN(...)
#endif

Servo myservo1; // 创建舵机对象

// NODEMCU版本引脚
int MotorA1 = 4; // 前后轮子
int MotorA2 = 3;
// 接收机引脚
#define PS2_DAT 13 //14
#define PS2_CMD 12 //15
#define PS2_CS 11  //16
#define PS2_CLK 10 //17
// 舵机引脚
#define SERVO_PIN_1 7
#define STOP 0
// analogWrite(pin, value)  UNO:0-255  D1 ESP8266:0-1023
#define MAX_SPEED 255
#define MID_SPEED 128
#define LOW_SPEED 64
#define START_SPEED 20

/******************************************************************
   select modes of PS2 controller:
     - pressures = analog reading of push-butttons
     - rumble    = motor rumbling
   uncomment 1 of the lines for each mode selection
 ******************************************************************/
#define pressures false
#define rumble false

PS2X ps2x; // create PS2 Controller Class

int RUN_SPEED = 0; // 推进速度
int LR = 0;        // 转向速度
int MAX_RUN_SPEED = MID_SPEED;
unsigned long starttime;
unsigned long looptime;

// void go_poweroff()
// {
//     Serial.print("go to sleep!!!");
//     delay(1000);

// #ifdef ESP8266
//     ESP.deepSleep(30000e6);
// #else
//     pinMode(13, OUTPUT);   // UNO板载LED关闭
//     digitalWrite(13, LOW); // UNO板载LED关闭
//     // 采用“Power-down”睡眠模式
//     set_sleep_mode(SLEEP_MODE_PWR_DOWN);
//     // 启用睡眠模式
//     sleep_enable();
//     // 进入睡眠模式
//     sleep_cpu();
// #endif
// }
void setup()
{
    Serial.begin(115200);

    // 去除pwm啸叫
    // analogWriteFreq(40e3);
    TCCR2B = TCCR2B & 0xF8 | 1;

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
            LOG(" uptime:");
            LOG(millis());
            LOG(" ");
            if (millis() > 600000) // 启动时检测不到手柄连接器，超过600秒，进入睡眠
            {
                // go_poweroff();
            }
            delay(100);
        }
    } while (1);

    // 电机驱动引脚初始化
    pinMode(MotorA1, OUTPUT);
    pinMode(MotorA2, OUTPUT);

    // 舵机引脚初始化
    myservo1.attach(SERVO_PIN_1);

    starttime = millis();
}

void loop()
{
    // 速度初始化
    RUN_SPEED = STOP;
    LR = STOP;

    ps2x.read_gamepad(false, 0); //read controller and set large motor to spin at 'vibrate' speed

    if (ps2x.Button(PSB_R2))
    {
        MAX_RUN_SPEED = MAX_SPEED;
    }
    else if (ps2x.ButtonReleased(PSB_R2))
    {
        MAX_RUN_SPEED = MID_SPEED;
    }

    // 高速锁定切换
    if (ps2x.ButtonReleased(PSB_L3))
    {
        MAX_RUN_SPEED = MAX_RUN_SPEED > MID_SPEED ? MID_SPEED : MAX_SPEED;
    }

    // 左边按键群
    if (ps2x.Button(PSB_PAD_UP))
    { //will be TRUE as long as button is pressed
        // LOGLN("Up held this hard: ");
        RUN_SPEED = MAX_RUN_SPEED;
    }
    else if (ps2x.ButtonReleased(PSB_PAD_UP))
    {
        // LOGLN("Up Button Released!");
        RUN_SPEED = STOP;
    }

    if (ps2x.Button(PSB_PAD_DOWN))
    {
        // LOGLN("DOWN held this hard: ");
        RUN_SPEED = -MAX_RUN_SPEED;
    }
    else if (ps2x.ButtonReleased(PSB_PAD_DOWN))
    {
        // LOGLN("DOWN Button Released!");
        RUN_SPEED = STOP;
    }

    // if (ps2x.Button(PSB_PAD_RIGHT))
    // {
    //     // LOGLN("Right held this hard: ");
    //     LR = -MAX_SPEED;
    // }
    // else if (ps2x.ButtonReleased(PSB_PAD_RIGHT))
    // {
    //     // LOGLN("Right Button Released!");
    //     LR = STOP;
    // }

    // if (ps2x.Button(PSB_PAD_LEFT))
    // {
    //     // LOGLN("LEFT held this hard: ");
    //     LR = MAX_SPEED;
    // }
    // else if (ps2x.ButtonReleased(PSB_PAD_LEFT))
    // {
    //     // LOGLN("LEFT Button Released!");
    //     LR = STOP;
    // }

    // 右边按键群
    if (ps2x.Button(PSB_TRIANGLE))
    { //will be TRUE as long as button is pressed
        // LOGLN("Triangle pressed");
        RUN_SPEED = MAX_RUN_SPEED;
    }
    else if (ps2x.ButtonReleased(PSB_TRIANGLE))
    {
        // LOGLN("Triangle Button Released!");
        RUN_SPEED = STOP;
    }

    if (ps2x.Button(PSB_CROSS))
    {
        // LOGLN("PSB_CROSS pressed");
        RUN_SPEED = -MAX_RUN_SPEED;
    }
    else if (ps2x.ButtonReleased(PSB_CROSS))
    {
        // LOGLN("PSB_CROSS Button Released!");
        RUN_SPEED = STOP;
    }

    if (ps2x.Button(PSB_CIRCLE)) // 右
    {
        // LOGLN("PSB_CIRCLE pressed");
        LR = -MAX_SPEED;
    }
    else if (ps2x.ButtonReleased(PSB_CIRCLE))
    {
        // LOGLN("PSB_CIRCLE Button Released!");
        LR = STOP;
    }

    if (ps2x.Button(PSB_SQUARE)) // 左
    {
        // LOGLN("PSB_SQUARE pressed");
        LR = MAX_SPEED;
    }
    else if (ps2x.ButtonReleased(PSB_SQUARE))
    {
        // LOGLN("PSB_SQUARE Button Released!");
        LR = STOP;
    }

    if (ps2x.Button(PSB_R2))
    {
        MAX_RUN_SPEED = MAX_SPEED;
    }
    else if (ps2x.ButtonReleased(PSB_R2))
    {
        MAX_RUN_SPEED = MID_SPEED;
    }
    // // 翻斗
    // if (ps2x.Button(PSB_R1)) // 右
    // {
    //     LOGLN("PSB_R1 pressed");
    //     myservo1.write(0);
    //     delay(10);
    //     return;
    // }
    // else if (ps2x.ButtonReleased(PSB_R1))
    // {
    //     LOGLN("PSB_R1 Button Released!");
    //     myservo1.write(170);
    // }
    if (RUN_SPEED == STOP)
    {
        RUN_SPEED = map(constrain((int)ps2x.Analog(PSS_LY), 0, 255), 0, 255, MAX_RUN_SPEED, -MAX_RUN_SPEED);
    }
    LOG("RUN:");
    LOG(RUN_SPEED);
    int v_abs = abs(RUN_SPEED);
    v_abs = constrain(v_abs, 0, MAX_RUN_SPEED);
    if (v_abs < START_SPEED) // 小于启动速度，不要发送PWM
    {
        digitalWrite(MotorA1, HIGH);
        digitalWrite(MotorA2, HIGH);
    }
    else if (RUN_SPEED > STOP)
    {
        digitalWrite(MotorA1, LOW);
        analogWrite(MotorA2, v_abs);
    }
    else if (RUN_SPEED < STOP)
    {
        digitalWrite(MotorA1, HIGH);
        analogWrite(MotorA2, MAX_SPEED - v_abs);
    }
    else
    {
        digitalWrite(MotorA1, LOW);
        digitalWrite(MotorA2, LOW);
    }

    if (LR > STOP)
    {
        LR = 75;
    }
    else if (LR < STOP)
    {
        LR = 130;
    }
    else
    {
        LR = map(constrain((int)ps2x.Analog(PSS_RX), 0, 255), 0, 255, 75, 130);
    }
    LOG(" LR:");
    LOG(LR);
    myservo1.write(LR);
    LOGLN();
    delay(10);
    return;

    if (RUN_SPEED == STOP && LR == STOP) // 在按键全部释放
    {
        // RUN_SPEED = map(constrain((int)ps2x.Analog(PSS_LY), 0, 255), 0, 255, 255, 0);
        // LR = map(constrain((int)ps2x.Analog(PSS_RX), 0, 255), 0, 255, 0, 255);
        // if ((RUN_SPEED < (Y_MID + SILL) && RUN_SPEED > (Y_MID - SILL)) && (LR < (X_MID + SILL) && LR > (X_MID - SILL)))
        // {
        //     mrun.two(STOP, STOP);
        //     if ((millis() - starttime) > 600000)
        //     { // xx秒没有按，自动关机 10分钟
        //         go_poweroff();
        //     }
        // }
        // else
        // {
        //     starttime = millis();
        //     mrun.tank(RUN_SPEED, LR);
        // }
        // if (TANK_MOD)
        // {
        //     RUN_SPEED = map(constrain((int)ps2x.Analog(PSS_LY), 0, 254), 0, 254, MAX_SPEED, -MAX_SPEED);
        //     LR = map(constrain((int)ps2x.Analog(PSS_RY), 0, 254), 0, 254, MAX_SPEED, -MAX_SPEED);
        //     mrun.two(RUN_SPEED, LR);
        // }
        // else
        // {
        // 右摇杆
        // RUN_SPEED = map(constrain((int)ps2x.Analog(PSS_RY), 0, 255), 0, 255, 255, 0);
        // LR = map(constrain((int)ps2x.Analog(PSS_RX), 0, 255), 0, 255, 0, 255);

        // if (RUN_SPEED == 128 && LR == 128) // R摇杆不在控制
        // {
        //     // 左摇杆
        //     // RUN_SPEED = map(constrain((int)ps2x.Analog(PSS_LY), 0, 255), 0, 255, 255, 0);
        //     // LR = map(constrain((int)ps2x.Analog(PSS_LX), 0, 255), 0, 255, 0, 255);
        //     // mrun.tank_v2(RUN_SPEED, LR);

        //     // 摇杆中位时舵机打到170度,上下都可控制
        //     int LYnum = constrain((int)ps2x.Analog(PSS_LY), 0, 255);
        //     if (LYnum < Y_MID)
        //     {
        //         RUN_SPEED = map(LYnum, Y_MID, 0, 170, 0);
        //     }
        //     else
        //     {
        //         RUN_SPEED = map(LYnum, Y_MID, 255, 170, 0);
        //     }
        //     LR = map(constrain((int)ps2x.Analog(PSS_LX), 0, 255), 0, 255, 0, 180);
        //     myservo1.write(RUN_SPEED);
        //     myservo2.write(LR);
        //     mrun.tank_v2(Y_MID, X_MID);
        // }
        // else
        // {
        //     mrun.tank(RUN_SPEED, LR);
        // }
        // }
    }
    else
    {
    }

    LOGLN();
    delay(10);
}
