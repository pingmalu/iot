#include <Servo.h>
Servo myservo;  // create servo object to control a servo
Servo myservo2; // create servo object to control a servo
int pos = 0;    // variable to read the value from the analog pin
String inString = "";
int old_1 = 0;
int old_2 = 0;

// 舵机引脚
#define SERVO_PIN_1 0
#define SERVO_PIN_2 2

void setup()
{
  Serial.begin(9600);
  myservo.attach(SERVO_PIN_1);  // attaches the servo on pin 9 to the servo object
  myservo2.attach(SERVO_PIN_2); // attaches the servo on pin 9 to the servo object
  pos = 0;
  old_1 = 0;
}

void loop()
{
  pos = 0;
  old_1 = 0;
  //
  //myservo2.write(180);
  //delay(999999);

  // Read serial input:
  while (Serial.available() > 0)
  {
    int inChar = Serial.read();
    if (isDigit(inChar))
    {
      // convert the incoming byte to a char and add it to the string:
      inString += (char)inChar;
    }
    // if you get a newline, print the string, then the string's value:
    if (inChar == '\n')
    {
      Serial.print("Value:");
      old_1 = inString.toInt();
      Serial.println(old_1);
      Serial.print("String: ");
      Serial.println(inString);
      // clear the string for new input:
      inString = "";
      myservo.write(old_1);
      myservo2.write(old_1);
      delay(1000);
      // for (pos = 0; pos < old_1; pos += 1)
      // {
      //   myservo.write(pos);
      //   myservo2.write(pos);
      //   Serial.println(pos);
      //   delay(5);
      // }
      // for (pos = old_1; pos >= 0; pos -= 1)
      // {
      //   myservo.write(pos);
      //   myservo2.write(pos);
      //   Serial.println(pos);
      //   delay(5);
      // }
    }
  }

  // for (pos = 0; pos < 180; pos += 1)
  // {
  //   myservo.write(pos);
  //   myservo2.write(pos);
  //   delay(15);
  // }
  // for (pos = 180; pos >= 1; pos -= 1)
  // {
  //   myservo.write(pos);
  //   myservo2.write(pos);
  //   delay(15);
  // }
}
