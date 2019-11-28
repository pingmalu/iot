/**
 * 远程自动避障坦克
 * BY: Malu
 * https://malu.me
 * 2018.12.14
 */

#include <ESP8266WiFi.h>

#include <Servo.h>
//#include <IRremote.h>

#define STOP      0
#define FORWARD   1
#define BACKWARD  2
#define TURNLEFT  3
#define TURNRIGHT 4

// 电机速度定义
#define SPEED_1 800
#define SPEED_2 100
#define SPEED_3 150

// 矫正舵机中位
#define SERVO_1 100

Servo myServo;  // 舵机

// 针脚定义
int EchoPin=D2;   // 收到反射回来的超声波
int TrigPin=D3;  // 发出超声波

int leftMotor1 = D4;  // 左边轮子
int leftMotor2 = D5;
int rightMotor1 = D7;  // 右边轮子
int rightMotor2 = D6;

int servoPin = D8;  // 舵机针脚，棕色为地，红色为电源正，橙色为信号线

int irPin = D9;  // 红外接收

// 红外初始化
//IRrecv irrecv(irPin);
//decode_results results;

int runtag = 0;

// 随机数
long randNumber;


#ifndef STASSID
#define STASSID "M2"
#define STAPSK  "passwd"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

IPAddress ip(192, 168, 0, 40);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(192, 168, 0, 1);

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);
int val;

void setup() {
  Serial.begin(9600);

  // prepare LED
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, 1);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print(F("Connecting to "));
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.config(ip, gateway, subnet, dns);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  Serial.println();
  Serial.println(F("WiFi connected"));

  // Start the server
  server.begin();
  Serial.println(F("Server started"));

  // Print the IP address
  Serial.println(WiFi.localIP());


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
//  irrecv.enableIRIn();


}

void loop() {
//  return;

  // if (irrecv.decode(&results)) {
  //   Serial.println(results.value,HEX);
  //   irrecv.resume();
  // }



  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  Serial.println(F("new client"));

  client.setTimeout(5000); // default is 1000

  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(F("request: "));
  Serial.println(req);

  // Match the request
  if (req.indexOf(F("/gpio/0")) != -1) {
    val = 0;
  } else if (req.indexOf(F("/gpio/1")) != -1) {
    val = 1;
  } else if (req.indexOf(F("/gpio/2")) != -1) {
    val = 2;
  } else if (req.indexOf(F("/gpio/3")) != -1) {
    val = 3;
  } else if (req.indexOf(F("/gpio/4")) != -1) {
    val = 4;
  } else if (req.indexOf(F("/gpio/5")) != -1) {
    val = 5;
  } else {
    Serial.println(F("invalid request"));
//    val = digitalRead(LED_BUILTIN);
  }

  // Set LED according to the request
  digitalWrite(LED_BUILTIN, val);

  // read/ignore the rest of the request
  // do not client.flush(): it is for output only, see below
  while (client.available()) {
    // byte by byte is not very efficient
    client.read();
  }

  // Send the response to the client
  // it is OK for multiple small client.print/write,
  // because nagle algorithm will group them into one single packet
  client.print(F("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html><head><meta http-equiv=Content-Type content=\"text/html;charset=utf-8\"></head>\r\n "));
  client.print(F("<style>body { font-size: 100px;}</style>"));
  client.print((val));
  client.print(F("<br>"));
  
  client.print(F("<a href='http://"));
  client.print(WiFi.localIP());
  client.print(F("/gpio/1'>前进</a>  "));

  client.print(F("<a href='http://"));
  client.print(WiFi.localIP());
  client.print(F("/gpio/2'>←</a>  "));

  client.print(F("<a href='http://"));
  client.print(WiFi.localIP());
  client.print(F("/gpio/3'>→</a>  "));

  client.print(F("<a href='http://"));
  client.print(WiFi.localIP());
  client.print(F("/gpio/4'>后退</a>  "));

  client.print(F("<a href='http://"));
  client.print(WiFi.localIP());
  client.print(F("/gpio/0'>停止</a> </html>"));

  // The client will actually be *flushed* then disconnected
  // when the function returns and 'client' object is destroyed (out-of-scope)
  // flush = ensure written data are received by the other side
  Serial.println(F("Disconnecting from client"));
  

  switch(val) {
    case 9:
//      avoidance();
      break;
    case 1:
      motor_f();
      break;
    case 2:
      motor_l();
      break;
    case 3:
      motor_r();
      break;
    case 4:
      motor_b();
      break;
    default:
      motorRun(STOP,0);
      break;
  }
}

void motor_f(){
  motorRun(FORWARD,SPEED_1);
}
void motor_l(){
  motorRun(TURNLEFT,SPEED_1);
}
void motor_r(){
  motorRun(TURNRIGHT,SPEED_1);
}
void motor_b(){
  motorRun(BACKWARD,SPEED_1);
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
