/**
 * Transmitter 发射机 (遥控器)
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
 */

#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>

const int VRX=A0;
const int VRY=A1;
const int SW = A2;
const int X0=525;
const int Y0=496;
byte joyStick[3];

void setup(){
  Serial.begin(115200);
  
  pinMode(SW, INPUT);
  pinMode(VRX, INPUT);
  pinMode(VRY, INPUT);
  // digitalWrite(SW, LOW);

  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
  Mirf.setRADDR((byte *)"clie1");
  Mirf.payload = sizeof(joyStick);
  Mirf.config();  

  Serial.println("Transmitter ready");
}

void loop() {

  int val_x =map(analogRead(VRX),0,1024,0,255);
  // int val_x =analogRead(VRX);
  int val_y=map(analogRead(VRY),0,1024,0,255);
  // int val_y=analogRead(VRY);
  int val_sw=map(analogRead(SW),0,1024,0,255);
  // int val_sw = analogRead(SW);

  //技巧:有多少个数就用多少个数组单元去储存
  
  joyStick[0] = val_x;
  joyStick[1] = val_y;
  joyStick[2] = val_sw;

  //这个时间值用于判断是否连接超时
  //unsigned long time = millis();
  //long time_long=long(time);
  
  Serial.print("SW:");
  Serial.print(val_sw);
  Serial.print(" X:");
  Serial.print(val_x);
  Serial.print(" Y:");
  Serial.println(val_y);

  // delay(50);

  //设置当前发射器的地址 
  Mirf.setTADDR((byte *)"serv1");

  //将读入的值写到SPI中
  Mirf.send((byte *) &joyStick);

  while(Mirf.isSending()) {
      // 如果正在发送时什么都不做，这里只是一个等待，等isSending完成后才能退出循环
  }
}