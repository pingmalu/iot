#include <Servo.h>
Servo myservo;  // create servo object to control a servo
Servo myservo2; // create servo object to control a servo
Servo myservo3; // create servo object to control a servo
Servo myservo4; // create servo object to control a servo
int pos = 0;    // variable to read the value from the analog pin

void setup()
{
  Serial.begin(9600);
  myservo.attach(5); // attaches the servo on pin 9 to the servo object
  myservo2.attach(4); // attaches the servo on pin 9 to the servo object
  myservo3.attach(0); // attaches the servo on pin 9 to the servo object
  myservo4.attach(2); // attaches the servo on pin 9 to the servo object
}

void loop()
{
    myservo.write(60);
    delay(1000);
    myservo2.write(60);
    delay(1000);
    myservo3.write(60);
    delay(1000);
    myservo4.write(60);
    delay(1000);
    myservo.write(120);
    delay(1000);
    myservo2.write(120);
    delay(1000);
    myservo3.write(120);
    delay(1000);
    myservo4.write(120);
    delay(1000);



  //  myservo.write(180);
  //myservo2.write(180);
  //delay(999999);
  // for (pos = 60; pos < 110; pos += 1)
  // {
  //   myservo.write(pos);
  //   myservo2.write(pos);
  //   delay(35);
  //   Serial.print("Value:");
  // }
  // for (pos = 110; pos >= 60; pos -= 1)
  // {
  //   myservo.write(pos);
  //   myservo2.write(pos);
  //   delay(35);
  // }

  // for (pos = 1; pos < 110; pos += 1)
  // {
  //   myservo3.write(pos);
  //   delay(35);
  // }
  // for (pos = 110; pos >= 1; pos -= 1)
  // {
  //   myservo3.write(pos);
  //   delay(35);
  // }

  // for (pos = 30; pos < 170; pos += 1)
  // {
  //   myservo4.write(pos);
  //   delay(35);
  // }
  // for (pos = 170; pos >= 30; pos -= 1)
  // {
  //   myservo4.write(pos);
  //   delay(35);
  // }
}
