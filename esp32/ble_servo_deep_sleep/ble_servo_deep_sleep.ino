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
char auth[] = "********_mk3_";
Servo servo;

BlynkTimer timer;

bool IS_CONNECT = false;
bool IS_OPENDED = false;

void open()
{
    //you begin your own personal code for servo here
    Serial.print("open\n");
    int pos;
    servo.write(180);
    delay(1000);
    servo.write(0);
    delay(1000);
    IS_OPENDED = true;
}

BLYNK_WRITE(V3)
{
    int pinValue = param.asInt();
    if (pinValue == 1)
    { // if Button sends 1
        Serial.print("1\n");
        open();                // start the function cradle
        Blynk.run();           // Run rest of show in-between waiting for this loop to repeat or quit.
        int pinValue = 0;      // Set V3 status to 0 to quit, unless button is still pushed (as per below)
        Blynk.syncVirtual(V3); // ...Then force BLYNK_WRITE(V3) function check of button status to determine if repeating or done.
    }
}

void goToSleep()
{
    IS_CONNECT = Blynk.connected();
    if (IS_CONNECT == false)
    {
        Serial.println("I'm awake, but I'm going into deep sleep mode for 20 seconds");
        ESP.deepSleep(20e6);
    }
}

void check_is_connected()
{
    IS_CONNECT = Blynk.connected();
    if (IS_CONNECT)
    {
        Serial.print("connected\n");
        if (!IS_OPENDED)
        {
            open();
        }
    }
}

void setup()
{
    // Debug console
    Serial.begin(115200);

    Serial.println("Waiting for connections...");

    Blynk.setDeviceName("R");

    Blynk.begin(auth);

    servo.attach(13); //attaches servo to pin 13

    timer.setInterval(1000L, check_is_connected); // 1秒检查一下是否连接蓝牙
    timer.setInterval(30000L, goToSleep);
}

void loop()
{
    Blynk.run();
    timer.run();
}
