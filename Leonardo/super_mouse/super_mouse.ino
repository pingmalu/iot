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
//        // Work with nunchuk_data
        // nunchuk_print();
        X=(nunchuk_joystickX()+3)*0.1;
        Y=-(nunchuk_joystickY()+6)*0.1;
        Serial.print(X);
        Serial.println(Y);
        if(X!=0 || Y!=0){
            Mouse.move(X, Y, 0);
            delay(10);
        }
//        Serial.println("9");
    }
//    delay(2000);
}
