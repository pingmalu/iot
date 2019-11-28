/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Social networks:            http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  This example shows how to use ESP32 BLE
  to connect your project to Blynk.

  Warning: Bluetooth support is in beta!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#define BLYNK_USE_DIRECT_CONNECT

#include <BlynkSimpleEsp32_BLE.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <Servo.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "Ma61_mk3_";
Servo servo;

void cradle()
{
    //you begin your own personal code for servo here
    int pos;

    for (pos = 0; pos <= 180; pos += 1)
    { // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
        servo.write(pos); // tell servo to go to position in variable 'pos'
        delay(5);         // waits 15ms for the servo to reach the position
    }
    for (pos = 180; pos >= 0; pos -= 1)
    {                     // goes from 180 degrees to 0 degrees
        servo.write(pos); // tell servo to go to position in variable 'pos'
        delay(10);        // waits 15ms for the servo to reach the position
    }
    //your personal code for servo should end here
}

BLYNK_WRITE(V3)
{
    int pinValue = param.asInt();
    if (pinValue == 1)
    {                          // if Button sends 1
        cradle();              // start the function cradle
        Blynk.run();           // Run rest of show in-between waiting for this loop to repeat or quit.
        int pinValue = 0;      // Set V3 status to 0 to quit, unless button is still pushed (as per below)
        Blynk.syncVirtual(V3); // ...Then force BLYNK_WRITE(V3) function check of button status to determine if repeating or done.
    }
}

void setup()
{
    // Debug console
    Serial.begin(9600);

    Serial.println("Waiting for connections...");

    Blynk.setDeviceName("Blynk");

    Blynk.begin(auth);

    servo.attach(13); //attaches servo to pin 13
}

void loop()
{
    Blynk.run();
}
