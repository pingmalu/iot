/**
 * 自动避障坦克 pro
 * BY: Malu
 * https://malu.me
 * 2018.12.12
 */

#include <Servo.h>
#include <IRremote.h>

#define STOP      0
#define FORWARD   1
#define BACKWARD  2
#define TURNLEFT  3
#define TURNRIGHT 4

// 电机速度定义
#define SPEED_1 200
#define SPEED_2 100
#define SPEED_3 150

// 矫正舵机中位
#define SERVO_1 100

Servo myServo;  // 舵机

// 针脚定义
int EchoPin=2;   // 收到反射回来的超声波
int TrigPin=3;  // 发出超声波

int leftMotor1 = 4;  // 左边轮子
int leftMotor2 = 5;
int rightMotor1 = 7;  // 右边轮子
int rightMotor2 = 6;

int servoPin = 8;  // 舵机针脚，棕色为地，红色为电源正，橙色为信号线

int irPin = 9;  // 红外接收

// 红外初始化
IRrecv irrecv(irPin);
decode_results results;

int runtag = 0;

// 随机数
long randNumber;

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
  // 红外引脚初始化
  irrecv.enableIRIn();
}

void loop() {

  if (irrecv.decode(&results)) {
    Serial.println(results.value,HEX);
    irrecv.resume();

    if(results.value!=0xFFFFFFFF && results.value!=0) {
      if(results.value==0xF377C5B7) {
        runtag = 1;  // 自动驾驶
      }else if(results.value==0xD538681B) {
        runtag = 2;  // 停止
      }else if(results.value==0x488F3CBB) {
        runtag = 3;  // 前
      }else if(results.value==0x410109DB) {
        runtag = 4;  // 左
      }else if(results.value==0x7EC31EF7) {
        runtag = 5;  // 右
      }else if(results.value==0xA23C94BF) {
        runtag = 6;  // 后
      }
    }
    results.value = 0;

    switch(runtag) {
      case 1:
        avoidance();
        break;
      case 3:
        motor_f();
        break;
      case 4:
        motor_l();
        break;
      case 5:
        motor_r();
        break;
      case 6:
        motor_b();
        break;
      default:
        motorRun(STOP,0);
        break;
    }
  }else{  // 没有接到信号
    if(runtag==1){
      avoidance();
    }else{
      motorRun(STOP,0);
    }
  }

}

void motor_f(){
  motorRun(FORWARD,SPEED_1);
  delay(100);
}
void motor_l(){
  motorRun(TURNLEFT,SPEED_1);
  delay(100);
}
void motor_r(){
  motorRun(TURNRIGHT,SPEED_1);
  delay(100);
}
void motor_b(){
  motorRun(BACKWARD,SPEED_1);
  delay(100);
}

void motorRun(int cmd,int value) {
  switch(cmd) {
    case FORWARD:
      Serial.println("FORWARD"); //输出状态
      digitalWrite(leftMotor1, LOW);
      analogWrite(leftMotor2, value);
      digitalWrite(rightMotor1, LOW);
      analogWrite(rightMotor2, value);
      break;
    case BACKWARD:
      Serial.println("BACKWARD"); //输出状态
      digitalWrite(leftMotor1, HIGH);
      analogWrite(leftMotor2, 255-value);
      digitalWrite(rightMotor1, HIGH);
      analogWrite(rightMotor2, 255-value);
      break;
    case TURNLEFT:
      Serial.println("TURN  LEFT"); //输出状态
      digitalWrite(leftMotor1, HIGH);
      analogWrite(leftMotor2, 255-value);
      digitalWrite(rightMotor1, LOW);
      analogWrite(rightMotor2, value);
      break;
    case TURNRIGHT:
      Serial.println("TURN  RIGHT"); //输出状态
      digitalWrite(leftMotor1, LOW);
      analogWrite(leftMotor2, value);
      digitalWrite(rightMotor1, HIGH);
      analogWrite(rightMotor2, 255-value);
      break;
    default:
      Serial.print("."); //输出状态
      digitalWrite(leftMotor1, LOW);
      digitalWrite(leftMotor2, LOW);
      digitalWrite(rightMotor1, LOW);
      digitalWrite(rightMotor2, LOW);
      delay(50);
  }
}

void avoidance() {
  int pos;  // 舵机角度
  int dis[3];  // 距离

  myServo.write(SERVO_1);
  dis[1] = getDistance(); //中间

  if(dis[1]<10){ //判断障碍物距离，距离太近
    motorRun(BACKWARD,SPEED_2); //后退
    delay(300); //后退时间
    motorRun(STOP,0);
    return;
  }

  if(dis[1]<30 && dis[1]>=10) {
    motorRun(STOP,0);
    for (pos = SERVO_1; pos <= (SERVO_1+60); pos += 1) 
    {
      myServo.write(pos);
      delay(5);
    }
    dis[2]=getDistance(); //左边

    for (pos = (SERVO_1+60); pos >= (SERVO_1-60); pos -= 1)
    {
      myServo.write(pos);
      delay(5);
      // if(pos==90)
      //   dis[1]=getDistance(); //中间
    }
    dis[0]=getDistance();  //右边

    randNumber = random(1, 10);

    if(dis[0]<dis[2]) //右边距离障碍的距离比左边近
    {
      //左转
      motorRun(TURNLEFT,SPEED_2);

      // 回中
      for (pos = (SERVO_1-60); pos <= SERVO_1; pos += 1) 
      {
        myServo.write(pos);
        delay(5);
      }

      delay(200+(100*randNumber));
      return;
    }
    else  //右边距离障碍的距离比左边远
    {
      //右转
      motorRun(TURNRIGHT,SPEED_2);

      // 回中
      for (pos = (SERVO_1-60); pos <= SERVO_1; pos += 1) 
      {
        myServo.write(pos);
        delay(5);
      }

      delay(200+(100*randNumber));
      return;
    } 
  }

  if(dis[1]>200) {
    motorRun(FORWARD,SPEED_3);  //高速前进
  }else{
    motorRun(FORWARD,SPEED_2);  //前进
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
