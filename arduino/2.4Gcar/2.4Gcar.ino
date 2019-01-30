/**
 * 接收机
 *
 * Pins:
 * Hardware SPI:
 * MISO -> 12
 * MOSI -> 11
 * SCK -> 13
 *
 * Configurable:
 * CE -> 8
 * CSN -> 7
 *
 * Note: To see best case latency comment out all Serial.println
 * statements not displaying the result and load 
 * 'ping_server_interupt' on the server.
 */

#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>

#define STOP      0
#define FORWARD   1
#define BACKWARD  2
#define TURNLEFT  3
#define TURNRIGHT 4

// 电机速度定义
#define SPEED_1 1023
#define SPEED_2 100
#define SPEED_3 150

int leftMotor1 = 2;  // 左边轮子
int leftMotor2 = 3;
int rightMotor1 = 4;  // 右边轮子
int rightMotor2 = 5;

byte joyStick[3];

void setup() {
  
  Serial.begin(115200);

  // 电机驱动引脚初始化
  pinMode(leftMotor1, OUTPUT);
  pinMode(leftMotor2, OUTPUT);
  pinMode(rightMotor1, OUTPUT);
  pinMode(rightMotor2, OUTPUT);

  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();     
  Mirf.setRADDR((byte *)"serv1");   
  Mirf.payload = sizeof(joyStick);
  
  /*
   * Write channel and payload config then power up reciver.
   */
   
  /*
   * To change channel:
   * 
   * Mirf.channel = 10;
   *
   * NB: Make sure channel is legal in your area.
   */
   
  Mirf.config();

  Serial.println("Receiver ready");
  
}

void loop() {
  
  while(!Mirf.dataReady()){
  }

  Mirf.getData((byte *) &joyStick);
  
  int vx=joyStick[0];
  int vy=joyStick[1];
  int sw=joyStick[2];

  if (vy < 94) {
    motor_b();
  }   // step forward
  
  else if (vy > 100) {
    motor_f();
  }  // step backward

  else {
    // motorRun(STOP,0);
      if (vx < 94) {  
            motor_r();
      } // step left
      
      else if (vx > 100) {  
            motor_l();
      } // step right

      else {
        motorRun(STOP,0);
      }  // step stop
  }  // step stop
 

  
  Serial.print("SW:");
  Serial.print(sw);
  Serial.print(" X:");
  Serial.print(vx);
  Serial.print(" Y:");
  Serial.println(vy);  

}

void motor_f(){
  motorRun(FORWARD,SPEED_1);
  // delay(100);
}
void motor_l(){
  motorRun(TURNLEFT,SPEED_1);
  // delay(100);
}
void motor_r(){
  motorRun(TURNRIGHT,SPEED_1);
  // delay(100);
}
void motor_b(){
  motorRun(BACKWARD,SPEED_1);
  // delay(100);
}

void motorRun(int cmd,int value) {
  switch(cmd) {
    case FORWARD:
      Serial.println("FORWARD"); //输出状态
      digitalWrite(leftMotor1, LOW);
      digitalWrite(leftMotor2, HIGH);
      // analogWrite(leftMotor2, value);
      digitalWrite(rightMotor1, LOW);
      digitalWrite(rightMotor2, HIGH);
      // analogWrite(rightMotor2, value);
      break;
    case BACKWARD:
      Serial.println("BACKWARD"); //输出状态
      digitalWrite(leftMotor1, HIGH);
      analogWrite(leftMotor2, SPEED_1-value);
      digitalWrite(rightMotor1, HIGH);
      analogWrite(rightMotor2, SPEED_1-value);
      break;
    case TURNLEFT:
      Serial.println("TURN  LEFT"); //输出状态
      digitalWrite(leftMotor1, HIGH);
      analogWrite(leftMotor2, SPEED_1-value);
      digitalWrite(rightMotor1, LOW);
      analogWrite(rightMotor2, value);
      break;
    case TURNRIGHT:
      Serial.println("TURN  RIGHT"); //输出状态
      digitalWrite(leftMotor1, LOW);
      analogWrite(leftMotor2, value);
      digitalWrite(rightMotor1, HIGH);
      analogWrite(rightMotor2, SPEED_1-value);
      break;
    default:
      Serial.print("."); //输出状态
      digitalWrite(leftMotor1, LOW);
      digitalWrite(leftMotor2, LOW);
      digitalWrite(rightMotor1, LOW);
      digitalWrite(rightMotor2, LOW);
      // delay(50);
  }
}