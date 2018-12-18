/**
 * 遥控坦克
 * BY: Malu
 * https://malu.me
 * 2018.12.18
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

#define UNO_OR_D1 255

#define STOP 0
#define FORWARD 1
#define BACKWARD 2
#define TURNLEFT 3
#define TURNRIGHT 4

#define THRESHOLD 50
#define SPEED_X 0.26  // (信号/信号跨度)*(最大速度-起始驱动速度)

int pwm_v1;       // 1左右 脉冲值
int pwm_PIN1 = 8; // 1左右 引脚
int pwm_speed1 = 0;
int pwm_v2;       // 2上下 脉冲值
int pwm_PIN2 = 9; // 2上下 引脚
int pwm_speed2 = 0;

int START_SPEED = 70; //起始驱动速度
int MAX_SPEED = 200;  //最大速度
int M_of_S = 500;     //信号跨度

int leftMotor1 = 4; // 左边轮子 引脚
int leftMotor2 = 5;
int rightMotor1 = 7; // 右边轮子 引脚
int rightMotor2 = 6;

float leftspeed = 0;
float rightspeed = 0;

char show_v[8]; //串口输出用

void setup()
{
  Serial.begin(9600);       //串口波特率为9600
  pinMode(pwm_PIN1, INPUT); //设置引脚为输入模式
  pinMode(pwm_PIN2, INPUT); //设置引脚为输入模式

  // 电机驱动引脚初始化
  pinMode(leftMotor1, OUTPUT);
  pinMode(leftMotor2, OUTPUT);
}

void loop()
{
  // show_speed(-1423.1,-THRESHOLD);return;
  pwm_v1 = pulseIn(pwm_PIN1, HIGH); //读取引脚上的高电平脉冲，最大脉冲时间间隔为1秒，并且把结果返回 返回时间单位为微秒
  pwm_v2 = pulseIn(pwm_PIN2, HIGH); //读取引脚上的高电平脉冲，最大脉冲时间间隔为1秒，并且把结果返回 返回时间单位为微秒

  show_pwm(pwm_v1, pwm_v2);

  // return;

  pwm_speed1 = (pwm_v1 - 1480); // 1左右 中位1480
  pwm_speed2 = (pwm_v2 - 1485); // 2上下 中位1485

  if (abs(pwm_speed1) > 800 || abs(pwm_speed2) > 800)
  { // 最大信号阈值不要超过800
    motorRun(STOP, 0, 0);
    return;
  }

  if (abs(pwm_speed1) < THRESHOLD && abs(pwm_speed2) < THRESHOLD)
  {
    //停止
    motorRun(STOP, 0, 0);
    return;
  }
  else if (abs(pwm_speed1) < THRESHOLD && pwm_speed2 >= THRESHOLD)
  {
    //前进
    leftspeed = (abs(pwm_speed2) * SPEED_X) + START_SPEED;
    motorRun(FORWARD, leftspeed, leftspeed);
    return;
  }
  else if (abs(pwm_speed1) < THRESHOLD && pwm_speed2 <= -THRESHOLD)
  {
    //后退
    leftspeed = (abs(pwm_speed2) * SPEED_X) + START_SPEED;
    motorRun(BACKWARD, leftspeed, leftspeed);
    return;
  }
  else if (abs(pwm_speed2) < THRESHOLD && pwm_speed1 >= THRESHOLD)
  {
    //左转  pwm_PIN1 高电平
    leftspeed = (abs(pwm_speed1) * SPEED_X) + START_SPEED;
    motorRun(TURNLEFT, leftspeed, leftspeed);
    return;
  }
  else if (abs(pwm_speed2) < THRESHOLD && pwm_speed1 <= -THRESHOLD)
  {
    //右转
    leftspeed = (abs(pwm_speed1) * SPEED_X) + START_SPEED;
    motorRun(TURNRIGHT, leftspeed, leftspeed);
    return;
  }

  else if (pwm_speed1 >= THRESHOLD && pwm_speed2 >= THRESHOLD)
  {
    //左前
    leftspeed = (abs(pwm_speed2) * SPEED_X) + START_SPEED;
    rightspeed = (abs(pwm_speed2) * SPEED_X) + START_SPEED + (abs(pwm_speed1) * SPEED_X);
    motorRun(FORWARD, leftspeed, rightspeed);
    return;
  }
  else if (pwm_speed1 <= THRESHOLD && pwm_speed2 >= THRESHOLD)
  {
    //右前
    leftspeed = (abs(pwm_speed2) * SPEED_X) + START_SPEED + (abs(pwm_speed1) * SPEED_X);
    rightspeed = (abs(pwm_speed2) * SPEED_X) + START_SPEED;
    motorRun(FORWARD, leftspeed, rightspeed);
    return;
  }
  else if (pwm_speed1 >= THRESHOLD && pwm_speed2 <= -THRESHOLD)
  {
    //左后
    leftspeed = (abs(pwm_speed2) * SPEED_X) + START_SPEED;
    rightspeed = (abs(pwm_speed2) * SPEED_X) + START_SPEED + (abs(pwm_speed1) * SPEED_X);
    motorRun(BACKWARD, leftspeed, rightspeed);
    return;
  }
  else if (pwm_speed1 <= -THRESHOLD && pwm_speed2 <= -THRESHOLD)
  {
    //右后
    leftspeed = (abs(pwm_speed2) * SPEED_X) + START_SPEED + (abs(pwm_speed1) * SPEED_X);
    rightspeed = (abs(pwm_speed2) * SPEED_X) + START_SPEED;
    motorRun(BACKWARD, leftspeed, rightspeed);
    return;
  }
}

void show_pwm(int A, int B)
{
  Serial.print("pwm_PIN1:");
  Serial.print(A);
  Serial.print(" pwm_PIN2:");
  Serial.println(B);
}

void show_speed(float left_s, float right_s)
{
  Serial.print("L:");
  Serial.print(left_s);
  Serial.print(" R:");
  Serial.println(right_s);
}

void motorRun(int cmd, int valuel, int valuer)
{
  switch (cmd)
  {
  case FORWARD:
    Serial.println("FORWARD");
    show_speed(valuel, valuer);
    digitalWrite(leftMotor1, LOW);
    analogWrite(leftMotor2, valuel);
    digitalWrite(rightMotor1, LOW);
    analogWrite(rightMotor2, valuer);
    break;
  case BACKWARD:
    Serial.println("BACKWARD");
    show_speed(-valuel, -valuer);
    digitalWrite(leftMotor1, HIGH);
    analogWrite(leftMotor2, UNO_OR_D1 - valuel);
    digitalWrite(rightMotor1, HIGH);
    analogWrite(rightMotor2, UNO_OR_D1 - valuer);
    break;
  case TURNLEFT:
    Serial.println("LEFT");
    show_speed(-valuel, valuer);
    digitalWrite(leftMotor1, HIGH);
    analogWrite(leftMotor2, UNO_OR_D1 - valuel);
    digitalWrite(rightMotor1, LOW);
    analogWrite(rightMotor2, valuer);
    break;
  case TURNRIGHT:
    Serial.println("RIGHT");
    show_speed(valuel, -valuer);
    digitalWrite(leftMotor1, LOW);
    analogWrite(leftMotor2, valuel);
    digitalWrite(rightMotor1, HIGH);
    analogWrite(rightMotor2, UNO_OR_D1 - valuer);
    break;
  default:
    Serial.print(".");
    digitalWrite(leftMotor1, LOW);
    digitalWrite(leftMotor2, LOW);
    digitalWrite(rightMotor1, LOW);
    digitalWrite(rightMotor2, LOW);
    delay(50);
    break;
  }
}
