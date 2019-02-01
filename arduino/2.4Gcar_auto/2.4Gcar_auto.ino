/**
 * 自动寻迹小车
 * BY: Malu
 * https://malu.me
 * 2019.2.1
 */

// analogWrite(pin, value)  UNO:0-255  D1 Wifi:0-1023
#define UNO_OR_D1 255

// 电机速度定义
#define SPEED_1 100
#define SPEED_2 180
#define SPEED_3 255

int leftMotor1 = 2;  // 左边轮子
int leftMotor2 = 3;
int rightMotor1 = 4;  // 右边轮子
int rightMotor2 = 5;

int REDRCV1_PIN = D14;  // 红外接收，黑线传感器 UNO: 12; wemos D1: D14
int REDRCV2_PIN = D13;  // 红外接收，黑线传感器 UNO: 11; wemos D1: D13
int SW1_PIN = D15;  // 开关引脚  UNO: 13; wemos D1: D15

int SW1 = 0;  // 开关值
int SW1_STATE_tmp = 0;  // 开关信号值
int SW1_num_tmp = 0;   // 开关缓冲阈值，防止闪触

// 可调距离，接收到反射0，无接收（黑线）1
// 不可调节距离，hw-006 v1.2,接收到反射1,无接收（黑线）0
int REDRCV1 = 0;
int REDRCV2 = 0;

void Mrun_one(int v,int M1,int M2)
{
  int v_abs = abs(v);
  v_abs = v_abs<UNO_OR_D1?v_abs:UNO_OR_D1;
  if(v>0){
    digitalWrite(M1, LOW);
    analogWrite(M2, v_abs);
  }else if(v<0){
    digitalWrite(M1, HIGH);
    analogWrite(M2, UNO_OR_D1 - v_abs);
  }else{
    digitalWrite(M1, LOW);
    digitalWrite(M2, LOW);
  }
}

void Mrun(int vl=0, int vr=0)
{
  Mrun_one(vl,leftMotor1,leftMotor2);
  Mrun_one(vr,rightMotor1,rightMotor2);
  Serial.printf("%d %d \n",vl,vr);
}

void setup() {
  
  Serial.begin(115200);

  // 电机驱动引脚初始化
  pinMode(leftMotor1, OUTPUT);
  pinMode(leftMotor2, OUTPUT);
  pinMode(rightMotor1, OUTPUT);
  pinMode(rightMotor2, OUTPUT);

  // 红外接收，黑线传感器
  pinMode(REDRCV1_PIN, INPUT);
  pinMode(REDRCV2_PIN, INPUT);

  // attachInterrupt(digitalPinToInterrupt(SW1_PIN), auto_start, CHANGE);
}

void loop() {
  // Mrun();

  SW1_STATE_tmp = digitalRead(SW1_PIN); // 接高电平时触发

  // 开关阈值系统
  if(SW1_STATE_tmp==1){
    SW1_num_tmp++;
    return;
  }else{
    if(SW1_num_tmp>30){
      SW1=!SW1;
    }
    SW1_num_tmp=0;
  }

  // Serial.println(SW1);

  if(SW1==1){
    auto_start();
  }else{
    Mrun();  // 停止
    Serial.println("stop");
    delay(50);
  }

}

// 自动驾驶
void auto_start(){
  Serial.print("start ");
  REDRCV1 = digitalRead(REDRCV1_PIN);
  REDRCV2 = digitalRead(REDRCV2_PIN);
  Serial.print(REDRCV1);
  Serial.print(" ");
  Serial.println(REDRCV2);
  // Mrun();
}

