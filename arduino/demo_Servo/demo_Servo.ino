#include <Servo.h>
Servo myservo; // create servo object to control a servo
Servo myservo2; // create servo object to control a servo
int pos = 0; // variable to read the value from the analog pin

void setup() {
   myservo.attach(9); // attaches the servo on pin 9 to the servo object
   myservo2.attach(10); // attaches the servo on pin 9 to the servo object
}

void loop() {
//  myservo.write(180);
//myservo2.write(180);
//delay(999999);
  for(pos=0;pos<180;pos+= 1){
    myservo.write(pos);
    myservo2.write(pos);
    delay(15);
    }
   for(pos = 180;pos>=1;pos-=1){
    myservo.write(pos);
     myservo2.write(pos);
   delay(15);
   }


}
