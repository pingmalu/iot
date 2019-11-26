#include <Ps3Controller.h>
#include <Servo.h>

Servo myservo1; // create servo object to control a servo
Servo myservo2; // create servo object to control a servo
Servo myservo3; // create servo object to control a servo
Servo myservo4; // create servo object to control a servo

void notify()
{
    Serial.print(Ps3.data.status.battery);
    Serial.print(" ");
    Serial.print(Ps3.data.sensor.accelerometer.x);
    Serial.print(" ");
    Serial.print(Ps3.data.sensor.accelerometer.y);
    Serial.print(" ");
    Serial.print(Ps3.data.sensor.accelerometer.z);
    Serial.print(" ");
    Serial.print(Ps3.data.sensor.gyroscope.z);

    Serial.print(" ");
    Serial.print(Ps3.data.button.cross);
    Serial.print(" ");
    Serial.print(Ps3.data.button.square);
    Serial.print(" ");
    Serial.print(Ps3.data.button.triangle);
    Serial.print(" ");
    Serial.print(Ps3.data.button.circle);

    Serial.print(" ");
    Serial.print(Ps3.data.analog.button.cross);
    Serial.print(" ");
    Serial.print(Ps3.data.analog.button.square);
    Serial.print(" ");
    Serial.print(Ps3.data.analog.button.triangle);
    Serial.print(" ");
    Serial.print(Ps3.data.analog.button.circle);

    Serial.print(" ");
    Serial.print(Ps3.data.analog.stick.lx);
    Serial.print(" ");
    Serial.print(Ps3.data.analog.stick.ly);
    Serial.print(" ");
    Serial.print(Ps3.data.analog.stick.rx);
    Serial.print(" ");
    Serial.print(Ps3.data.analog.stick.ry);

    Serial.print(" ");
    Serial.print(Ps3.data.button.up);
    Serial.print(" ");
    Serial.print(Ps3.data.button.right);
    Serial.print(" ");
    Serial.print(Ps3.data.button.down);
    Serial.print(" ");
    Serial.print(Ps3.data.button.left);

    Serial.print(" ");
    Serial.print(Ps3.data.analog.button.up);
    Serial.print(" ");
    Serial.print(Ps3.data.analog.button.right);
    Serial.print(" ");
    Serial.print(Ps3.data.analog.button.down);
    Serial.print(" ");
    Serial.print(Ps3.data.analog.button.left);

    Serial.print(" ");
    Serial.print(Ps3.data.button.l1);
    Serial.print(" ");
    Serial.print(Ps3.data.button.r1);
    Serial.print(" ");
    Serial.print(Ps3.data.button.l2);
    Serial.print(" ");
    Serial.print(Ps3.data.button.r2);

    Serial.print(" ");
    Serial.print(Ps3.data.analog.button.l1);
    Serial.print(" ");
    Serial.print(Ps3.data.analog.button.r1);
    Serial.print(" ");
    Serial.print(Ps3.data.analog.button.l2);
    Serial.print(" ");
    Serial.print(Ps3.data.analog.button.r2);

    Serial.print(" ");
    Serial.print(Ps3.data.button.select);
    Serial.print(" ");
    Serial.print(Ps3.data.button.start);
    Serial.print(" ");
    Serial.print(Ps3.data.button.ps);

    /* Uncomment the following if you also want
       to display the gryoscope data: */
    // Serial.print(" ");
    // Serial.print(Ps3.data.sensor.gyroscope.z);

    Serial.println();

    //    Serial.print(map(constrain((int)Ps3.data.sensor.accelerometer.x, 1, 180), 1, 180, 1, 180));
    myservo1.write(map(constrain((int)Ps3.data.sensor.accelerometer.x, 1, 180), 1, 180, 1, 180));
    myservo2.write(map(constrain((int)Ps3.data.sensor.accelerometer.y, 1, 180), 1, 180, 1, 180));
    myservo3.write(map(constrain((int)Ps3.data.sensor.accelerometer.z, 1, 180), 1, 180, 1, 180));
}

void setup()
{
    myservo1.attach(22); // attaches the servo on pin 9 to the servo object
    myservo2.attach(23); // attaches the servo on pin 9 to the servo object
    myservo3.attach(1);  // attaches the servo on pin 9 to the servo object
    myservo4.attach(3);  // attaches the servo on pin 9 to the servo object

    Serial.begin(115200);
    Ps3.attach(notify);
    Ps3.begin("00:1a:7d:da:71:13");
}

void loop()
{
}
