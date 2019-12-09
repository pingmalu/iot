#include <Ps3Controller.h>
#include <Servo.h>

Servo myservo1;
Servo myservo2;
Servo myservo3;
Servo myservo4;

int16_t SVO1 = 90;
int16_t SVO2 = 90;
int16_t SVO3 = 90;
int16_t SVO4 = 90;

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

    // myservo1.write(map(constrain((int)Ps3.data.sensor.accelerometer.x, 1, 180), 1, 180, 1, 180));
    // myservo2.write(map(constrain((int)Ps3.data.sensor.accelerometer.y, 1, 180), 1, 180, 1, 180));
    // myservo3.write(map(constrain((int)Ps3.data.sensor.accelerometer.z, 1, 180), 1, 180, 1, 180));

    SVO1 = pr255(Ps3.data.analog.button.r2); // 夹子

    if (Ps3.data.button.left == 1)
    {
        if (SVO2 < 180)
            SVO2++;
    }
    if (Ps3.data.button.right == 1)
    {
        if (SVO2 > 0)
            SVO2--;
    }

    if (Ps3.data.button.up == 1)
    {
        if (SVO3 < 180)
            SVO3++;
    }
    if (Ps3.data.button.down == 1)
    {
        if (SVO3 > 0)
            SVO3--;
    }

    if (Ps3.data.button.cross == 1) //x
    {
        if (SVO4 < 180)
            SVO4++;
    }
    if (Ps3.data.button.triangle == 1) //A
    {
        if (SVO4 > 0)
            SVO4--;
    }

    // delay(15);
    myservo1.write(SVO1);
    myservo2.write(pr(Ps3.data.analog.stick.lx));
    myservo3.write(pr(Ps3.data.analog.stick.ly));
    myservo4.write(pr(Ps3.data.analog.stick.ry));
    // myservo3.write(SVO3);
    // myservo4.write(SVO4);

    Serial.print(" ( ");
    Serial.print(SVO2);
    Serial.print(" ");
    Serial.print(SVO3);
    Serial.print(" ");
    Serial.print(SVO4);
    Serial.print(" )");

    Serial.println();
}

int16_t pr(int16_t val)
{
    val = map(val, -128, 127, 0, 180);
    Serial.print(" [");
    Serial.print(val);
    Serial.print("]");
    return val;
}

int16_t pr255(int16_t val)
{
    val = map(val, 0, 255, 0, 180);
    Serial.print(" [");
    Serial.print(val);
    Serial.print("]");
    return val;
}

void setup()
{
    myservo1.attach(23);
    myservo2.attach(22);
    myservo3.attach(21);
    myservo4.attach(19);

    Serial.begin(115200);
    Ps3.attach(notify);
    Ps3.begin("FF:87:E0:A6:AC:05");
}

void loop()
{
}
