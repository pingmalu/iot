/**
 * 自动避障坦克
 * BY: Malu
 * https://malu.me
 * 2018.12.11
 */

#include <Servo.h>

#define STOP      0
#define FORWARD   1
#define BACKWARD  2
#define TURNLEFT  3
#define TURNRIGHT 4

Servo myServo;  // 舵机

// 针脚定义
int EchoPin=2;   // 收到反射回来的超声波
int TrigPin=3;  // 发出超声波

int leftMotor1 = 4;  // 左边轮子
int leftMotor2 = 5;
int rightMotor1 = 6;  // 右边轮子
int rightMotor2 = 7;

int servoPin = 8;  // 舵机针脚，棕色为地，红色为电源正，橙色为信号线

void setup() {
  // 串口初始化
  Serial.begin(9600);
  // 超声波控制引脚初始化
  pinMode(EchoPin, INPUT);
  pinMode(TrigPin, OUTPUT);
  // 舵机引脚初始化
  myServo.attach(servoPin);
  // 电机驱动引脚初始化
  pinMode(leftMotor1, OUTPUT);
  pinMode(leftMotor2, OUTPUT);
  pinMode(rightMotor1, OUTPUT);
  pinMode(rightMotor2, OUTPUT);
}

void loop() {
  avoidance();
}

void motorRun(int cmd,int value) {
  switch(cmd) {
    case FORWARD:
      Serial.println("FORWARD"); //输出状态
      digitalWrite(leftMotor1, HIGH);
      digitalWrite(leftMotor2, LOW);
      digitalWrite(rightMotor1, HIGH);
      digitalWrite(rightMotor2, LOW);
      break;
    case BACKWARD:
      Serial.println("BACKWARD"); //输出状态
      digitalWrite(leftMotor1, LOW);
      digitalWrite(leftMotor2, HIGH);
      digitalWrite(rightMotor1, LOW);
      digitalWrite(rightMotor2, HIGH);
      break;
    case TURNLEFT:
      Serial.println("TURN  LEFT"); //输出状态
      digitalWrite(leftMotor1, HIGH);
      digitalWrite(leftMotor2, LOW);
      digitalWrite(rightMotor1, LOW);
      digitalWrite(rightMotor2, HIGH);
      break;
    case TURNRIGHT:
      Serial.println("TURN  RIGHT"); //输出状态
      digitalWrite(leftMotor1, LOW);
      digitalWrite(leftMotor2, HIGH);
      digitalWrite(rightMotor1, HIGH);
      digitalWrite(rightMotor2, LOW);
      break;
    default:
      Serial.println("STOP"); //输出状态
      digitalWrite(leftMotor1, LOW);
      digitalWrite(leftMotor2, LOW);
      digitalWrite(rightMotor1, LOW);
      digitalWrite(rightMotor2, LOW);
  }
}

void avoidance() {
  int pos;  // 舵机角度
  int dis[3];  // 距离

  myServo.write(90);
  dis[1] = getDistance(); //中间

  if(dis[1]<10){ //判断障碍物距离，距离太近
    motorRun(BACKWARD,250); //后退
    delay(500); //后退时间
  }

  if(dis[1]<30 && dis[1]>=10) {
    motorRun(STOP,0);
    for (pos = 90; pos <= 150; pos += 1) 
    {
      myServo.write(pos);    // tell servo to go to position in variable 'pos'
      delay(15);             // waits 15ms for the servo to reach the position
    }
    dis[2]=getDistance(); //左边

    for (pos = 150; pos >= 30; pos -= 1)
    {
      myServo.write(pos);    // tell servo to go to position in variable 'pos'
      delay(15);             // waits 15ms for the servo to reach the position
      if(pos==90)
        dis[1]=getDistance(); //中间
    }

    dis[0]=getDistance();  //右边
    for (pos = 30; pos <= 90; pos += 1) 
    {
      myServo.write(pos);    // tell servo to go to position in variable 'pos'
      delay(15);             // waits 15ms for the servo to reach the position
    }

    if(dis[0]<dis[2]) //右边距离障碍的距离比左边近
    {
      //左转
      motorRun(TURNLEFT,250);
      delay(500);
    }
    else  //右边距离障碍的距离比左边远
    {
      //右转
      motorRun(TURNRIGHT,250);
      delay(500);
    } 
  }

  if(dis[1]>200) {
    motorRun(FORWARD,200);  //高速前进
  }else{
    motorRun(FORWARD,150);  //前进
  }
}

int getDistance() {
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
  distance = distance*0.017;  //脉冲时间转化为距离值

  Serial.print(distance); //输出距离值
  Serial.println('cm');

  return distance;
}
