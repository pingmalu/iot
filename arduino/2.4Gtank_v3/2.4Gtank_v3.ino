/**
 * 遥控坦克
 * BY: Malu
 * https://malu.me
 * 2019.2.22
 */

// pulseIn(pin, value, timeout)
// 1左右 脉冲范围 中：1621  低：2096  高：1113  抖动：50
// 2上下 脉冲范围 中：1360  低：869  高：1860  抖动：50
// 3油门 低：884  高：1853
// 4油门左右  中：1600  低：1108  高：2096
// 5 下：1977  中：1480  上：989
// 6 下：1977  上：989
// D/R 舵量

// 电机启动速度 100-200
// analogWrite(pin, value)  UNO:0-255  D1 Wifi:0-1023
#include "MRUN_lib.h"
int Y_MAX = 255;
int Y_MID = 127;
int Y_MIN = 0;
int X_MAX = 255;
int X_MID = 128;
int X_MIN = 0;
int SILL = 20; // 偏移阈值
MRUN mrun;
#ifndef ESP8266
#define MAX_SPEED 255
#else
#define MAX_SPEED 1023
#endif

int RUN_SPEED = 0; // 推进速度
int LR = 0;        // 转向速度
#define STOP 0


int pwm_v1;        // 1左右 脉冲值
int pwm_PIN1 = 8;  // 1左右 引脚
int pwm_v2;        // 2上下 脉冲值
int pwm_PIN2 = 9;  // 2上下 引脚
int pwm_PIN3 = 10; // 6CH 引脚
int pwm_v3;        // 6CH 脉冲值

int leftMotor1 = 4; // 左边轮子 引脚
int leftMotor2 = 5;
int rightMotor1 = 7; // 右边轮子 引脚
int rightMotor2 = 6;

void setup()
{
  Serial.begin(115200);     //串口波特率为9600
  pinMode(pwm_PIN1, INPUT); //设置引脚为输入模式
  pinMode(pwm_PIN2, INPUT); //设置引脚为输入模式

  // 电机驱动引脚初始化
  pinMode(leftMotor1, OUTPUT);
  pinMode(leftMotor2, OUTPUT);
  pinMode(rightMotor1, OUTPUT);
  pinMode(rightMotor2, OUTPUT);

  mrun.config(leftMotor1, leftMotor2, rightMotor1, rightMotor2, Y_MAX, Y_MID, Y_MIN, X_MAX, X_MID, X_MIN, SILL);
}

void loop()
{
  // 速度初始化
  RUN_SPEED = STOP;
  LR = STOP;
  // show_speed(-1423.1,-THRESHOLD);return;
  pwm_v1 = pulseIn(pwm_PIN1, HIGH); //读取引脚上的高电平脉冲，最大脉冲时间间隔为1秒，并且把结果返回 返回时间单位为微秒
  pwm_v2 = pulseIn(pwm_PIN2, HIGH); //读取引脚上的高电平脉冲，最大脉冲时间间隔为1秒，并且把结果返回 返回时间单位为微秒

  // 探照灯
  pwm_v3 = pulseIn(pwm_PIN3, HIGH);
  if (pwm_v3 > 1300)
  {
    // Serial.println(150);
    analogWrite(12, 150);
  }
  else
  {
    digitalWrite(12, LOW);
  }

  show_pwm(pwm_v1, pwm_v2);

  RUN_SPEED = map(constrain((int)pwm_v2, 1238, 1738), 1238, 1738, 0, MAX_SPEED);
  LR = map(constrain((int)pwm_v1, 1220, 1720), 1220, 1720, MAX_SPEED, 0);
  // Serial.print(" RUN_SPEED:");
  // Serial.print(RUN_SPEED);
  // Serial.print(" LR:");
  // Serial.print(LR);
  mrun.tank(RUN_SPEED, LR);
  delay(100);
}

void show_pwm(int A, int B)
{
  Serial.print(" pwm_PIN1:");
  Serial.print(A);
  Serial.print(" pwm_PIN2:");
  Serial.println(B);
}
