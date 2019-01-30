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

void setup() {

    Serial.begin(9600);
    Wire.begin();
    // nunchuk_init_power(); // A1 and A2 is power supply
    nunchuk_init();
    Keyboard.begin();
    Mouse.begin();
}

void loop() {
    if (nunchuk_read()) {
        // nunchuk_print();
        X=(nunchuk_joystickX()+3)*0.1;
        Y=-(nunchuk_joystickY()+6)*0.1;
        Z=nunchuk_buttonZ();
        C=nunchuk_buttonC();
        Serial.print(X);
        Serial.print(" ");
        Serial.print(Y);
        Serial.print(" ");
        Serial.print(Z);
        Serial.print(" ");
        Serial.print(C);
        Serial.println(" ");

        if (Z==0) {
            Z_NUM=0;
        }else{
            Z_NUM++;
        }

        if (Z==0) {
            if(X!=0 || Y!=0){
                Mouse.move(X, Y, 0);
                delay(10);
                return;
            }
        }else{ // Z 按下慢移动
            if(X!=0 || Y!=0){
                if(X<0)X=-1;
                if(X>0)X=1;
                if(Y<0)Y=-1;
                if(Y>0)Y=1;
                Mouse.move(X, Y, 0);
                delay(10);
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

        if (Z==1) {
            // if the mouse is not pressed, press it:
            if (!Mouse.isPressed(MOUSE_RIGHT)) {
                Mouse.press(MOUSE_RIGHT);
            }
        } else {                           // else the mouse button is not pressed:
            // if the mouse is pressed, release it:
            if (Mouse.isPressed(MOUSE_RIGHT)) {
                Mouse.release(MOUSE_RIGHT);
            }
        }

    }
//    delay(2000);
}
