int D;  //定义duration变量为无符号长整数型变量

#define STOP      0
#define FORWARD   1
#define BACKWARD  2
#define TURNLEFT  3
#define TURNRIGHT 4

int pwm_PIN1 = 8;
int START_SPEED = 70;  //起始驱动速度
int MAX_SPEED = 200;  //最大速度
int M_of_S = 500;  //信号跨度

int leftMotor1 = 4;  // 左边轮子
int leftMotor2 = 5;
int rightMotor1 = 7;  // 右边轮子
int rightMotor2 = 6;

int Dspeed = 0;
float leftspeed = 0;


void setup()
{
  Serial.begin(9600);  //串口波特率为9600
  pinMode(pwm_PIN1, INPUT); //设置引脚为输入模式

  // 电机驱动引脚初始化
  pinMode(leftMotor1, OUTPUT);
  pinMode(leftMotor2, OUTPUT);
} 

// pulseIn(pin, value, timeout)
// 1左右 脉冲范围 中：1621  低：2096  高：1113  抖动：50
// 2上下 脉冲范围 中：1360  低：869  高：1860  抖动：50
// 3油门 低：884  高：1853
// 4油门左右  中：1600  低：1108  高：2096
// 5 下：1977  中：1480  上：989
// 6 下：1977  上：989
// D/R 舵量

// 电机启动速度 100-200

void loop()
{
  D = pulseIn(pwm_PIN1, HIGH); //读取引脚上的高电平脉冲，最大脉冲时间间隔为1秒，并且把结果赋值给duration变量 返回时间单位为微秒
  Serial.println(D);

  Dspeed = (D-1471);  // 中位1471

  if(abs(Dspeed)>800){  // 最大信号阈值不要超过800
    motorRun(STOP,0);
    return;  
  }

  if(abs(Dspeed)<50){
    motorRun(STOP,0);
    return;
  }else if((Dspeed)>0){
    //前进
    leftspeed = (abs(Dspeed)*0.26)+START_SPEED;
    Serial.println(leftspeed);
    motorRun(FORWARD,leftspeed);
  }else{
    //后退
    leftspeed = (abs(Dspeed)*0.26)+START_SPEED;
    Serial.println(leftspeed);
    motorRun(BACKWARD,leftspeed);
  }

//  Serial.print(duration/1000000.0); //通过串口输出duration变量
//  Serial.println(" s");
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
      analogWrite(leftMotor2, 1023-value);
      digitalWrite(rightMotor1, HIGH);
      analogWrite(rightMotor2, 1023-value);
      break;
    case TURNLEFT:
      Serial.println("TURN  LEFT"); //输出状态
      digitalWrite(leftMotor1, HIGH);
      analogWrite(leftMotor2, 1023-value);
      digitalWrite(rightMotor1, LOW);
      analogWrite(rightMotor2, value);
      break;
    case TURNRIGHT:
      Serial.println("TURN  RIGHT"); //输出状态
      digitalWrite(leftMotor1, LOW);
      analogWrite(leftMotor2, value);
      digitalWrite(rightMotor1, HIGH);
      analogWrite(rightMotor2, 1023-value);
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
