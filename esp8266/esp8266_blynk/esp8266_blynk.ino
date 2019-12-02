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
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  This example runs directly on ESP8266 chip.

  Note: This requires ESP8266 support package:
    https://github.com/esp8266/Arduino

  Please be sure to select the right ESP8266 module
  in the Tools -> Board menu!

  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "qhDFWGZdcFk63Abua5V1aOJB7i02zLtI";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "16988";
char pass[] = "bric16988";

Servo myservo1;
Servo myservo2;
Servo myservo3;
Servo myservo4;

int16_t SVO1 = 90;
int16_t SVO2 = 90;
int16_t SVO3 = 90;
int16_t SVO4 = 90;

int16_t pr1024(int16_t val)
{
  val = map(val, 0, 1023, 0, 180);
  Serial.print(" [");
  Serial.print(val);
  Serial.print("]");
  return val;
}

BLYNK_WRITE(V1)
{
  int x = param[0].asInt();
  int y = param[1].asInt();

  // Do something with x and y
  Serial.print("X = ");
  Serial.print(x);
  Serial.print("; Y = ");
  Serial.println(y);

  myservo1.write(pr1024(x));
}

BLYNK_WRITE(V2)
{
  int x = param[0].asInt();
  int y = param[1].asInt();

  // Do something with x and y
  Serial.print("X = ");
  Serial.print(x);
  Serial.print("; Y = ");
  Serial.println(y);

  myservo2.write(pr1024(x));
}

void setup()
{
  // Debug console
  Serial.begin(9600);
  myservo1.attach(16);
  myservo2.attach(5);
  myservo3.attach(4);
  myservo4.attach(0);

  Blynk.begin(auth, ssid, pass);
}

void loop()
{
  Blynk.run();
}
