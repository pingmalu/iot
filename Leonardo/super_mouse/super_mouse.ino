/*
Leonardo 接线：

Top Left 		SCK		Digital Pin 3
Top Right		GND		Ground Pin
Bottom L.		+ V		5 Volt Pin
Bottom R.		SDA		Digital Pin 2

*/

#include <Wire.h>
#include "Nunchuk.h"
#include <Keyboard.h>
#include <Mouse.h>

float X=0;
float Y=0;
int C=0;
int Z=0;
int Z_NUM=0;
int Z_TYPE=0;

float AX=0;
float AY=0;
float AZ=0;

float AY_TMP=999; // AY首次按下初始位置

float Y_ALL=0;

void setup() {

    Serial.begin(115200);
    Wire.begin();
    // nunchuk_init_power(); // A1 and A2 is power supply
    nunchuk_init();
    Keyboard.begin();
    Mouse.begin();
}

void loop() {
    if (nunchuk_read()) {
        X=(nunchuk_joystickX()+3)*0.1;
        Y=-(nunchuk_joystickY()+6)*0.1;
        Z=nunchuk_buttonZ();
        C=nunchuk_buttonC();
        AX=nunchuk_accelX();
        AY=nunchuk_accelY();
        AZ=nunchuk_accelZ();
        Serial.print(X);
        Serial.print(" ");
        Serial.print(Y);
        Serial.print(" ");
        Serial.print(Z);
        Serial.print(" ");
        Serial.print(C);
        Serial.print(" ");
        nunchuk_print();

        if (Z==0) {
            if(Z_NUM<=0){
                Z_TYPE=0;
            }else if(Z_NUM>0 && Z_NUM<=200){
                Z_TYPE=1;  // 1:short 2:long
            }else if(Z_NUM>200){
                Z_TYPE=2;
            }
            Z_NUM=0;
        }else{
            Z_NUM++;
        }

        if (Z==0) { // Z C 都释放的时候
            if(X!=0 || Y!=0){
                if(AX<-250 && C==0){ //左斜放时下移
                    if(Y!=0){
                        Serial.print(Y_ALL);
                        if(Y_ALL<100 && Y_ALL>-100){
                            if (!Mouse.isPressed(MOUSE_LEFT)) {
                                Mouse.press(MOUSE_LEFT);
                                delay(50);
                                return;
                            }
                            // Y=(Y*0.1);
                            // if(Y>0){
                            //     Y=1;
                            // }else{
                            //     Y=-1;
                            // }
                            Mouse.move(0, Y, 0);
                            delay(5);
                            Y_ALL=Y_ALL+Y;
                            return;
                        }else{
                            if (Mouse.isPressed(MOUSE_LEFT)) {
                                Mouse.release(MOUSE_LEFT);
                                // delay(100);
                                return;
                            }
                            // delay(200);
                            // Y_ALL+=Y_ALL;
                            if(Y_ALL>0){ // 下正  正往下
                                // Y_ALL=Y_ALL+50;
                                // Mouse.move(0, -100, 0);
                                Mouse.move(0, -90, 0);
                                // Mouse.move(0, -127, 0);
                                // Mouse.move(0, -20, 0);
                                // Mouse.move(0, -(Y_ALL-100), 0);
                            }else{ // 上 上偏+  下偏-
                                // Y_ALL=Y_ALL-50;
                                // Y_ALL-=Y_ALL;
                                // Mouse.move(0, 100, 0);
                                Mouse.move(0, 90, 0);
                            }
                            // Mouse.move(0, Y_ALL, 0);
                            // delay(10);
                            // Mouse.move(0, Y_ALL, 0);
                            // Mouse.move(0, Y_ALL, 0);
                            // delay(200);
                            Y_ALL=0;
                            return;
                        }
                    }

                }else{
                    Mouse.move(X, Y, 0);
                    delay(5);
                    return;
                }
            }else{
                Y_ALL=0;
                if(AX<-250){
                    if(Y==0){
                        if (Mouse.isPressed(MOUSE_LEFT)) {
                            Mouse.release(MOUSE_LEFT);
                        }
                    }
                }
            }
            AY_TMP=999;
        }else if(AX>=-250 && Z==1){ // Z 按下,且非左斜放状态
            if(X!=0 || Y!=0){  // 慢移动
                if(X<0)X=-1;
                if(X>0)X=1;
                if(Y<0)Y=-1;
                if(Y>0)Y=1;
                Mouse.move(X, Y, 0);
                delay(10);
                return;
            }

            // 鼠标滚轮
            if(AY_TMP==999){  //记录第一次按下Z时的位置
                AY_TMP=AY;
                return;
            }
            if(AY>(AY_TMP+50)){
                Mouse.move(0, 0, 1);
                delay(100);
                return;
            }else if(AY<(AY_TMP-50)){
                Mouse.move(0, 0, -1);
                delay(100);
                return;
            }
        }

        if (C==1) {
            // if the mouse is not pressed, press it:
            if (!Mouse.isPressed(MOUSE_LEFT)) {
                Mouse.press(MOUSE_LEFT);
            }
        } else {                           // else the mouse button is not pressed:
            // if the mouse is pressed, release it:
            if (Mouse.isPressed(MOUSE_LEFT)) {
                Mouse.release(MOUSE_LEFT);
            }
        }

        if(AX<-250){  // 左斜放时，单机右键
            if (Z_TYPE==1) {
                // if the mouse is not pressed, press it:
                if (!Mouse.isPressed(MOUSE_RIGHT)) {
                    Mouse.press(MOUSE_RIGHT);
                }
            } else if(Z_TYPE==0) {                           // else the mouse button is not pressed:
                // if the mouse is pressed, release it:
                if (Mouse.isPressed(MOUSE_RIGHT)) {
                    Mouse.release(MOUSE_RIGHT);
                }
            } else if(Z_TYPE==2) {
                Keyboard.write(KEY_HOME);
            }
        }

    }
//    delay(2000);
}
