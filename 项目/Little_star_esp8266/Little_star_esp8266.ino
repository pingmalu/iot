
void setup()
{
    pinMode(D0, OUTPUT);
    pinMode(D1, OUTPUT);
    pinMode(D2, OUTPUT);
    pinMode(D3, OUTPUT);
    pinMode(D4, OUTPUT);
    pinMode(D5, OUTPUT);
    pinMode(D6, OUTPUT);
    pinMode(D7, OUTPUT);
    pinMode(D8, OUTPUT);
}

void loop()
{
    for (int i = 0; i < 823; i++)
    {
        analogWrite(D0, i);
        analogWrite(D1, i);
        analogWrite(D2, i);
        analogWrite(D3, i);
        analogWrite(D4, i);
        analogWrite(D5, i);
        analogWrite(D6, i);
        analogWrite(D7, i);
        analogWrite(D8, i);
        delay(random(1, 4));
    }
    for (int i = 823; i > 0; i--)
    {
        analogWrite(D0, i);
        analogWrite(D1, i);
        analogWrite(D2, i);
        analogWrite(D3, i);
        analogWrite(D4, i);
        analogWrite(D5, i);
        analogWrite(D6, i);
        analogWrite(D7, i);
        analogWrite(D8, i);
        delay(random(1, 4));
    }
    int i = 1023;
    int t;
    t = random(10,200);
    analogWrite(D0, i);delay(t);
    analogWrite(D1, i);delay(t);
    analogWrite(D2, i);delay(t);
    analogWrite(D3, i);delay(t);
    analogWrite(D4, i);delay(t);
    analogWrite(D5, i);delay(t);
    analogWrite(D6, i);delay(t);
    analogWrite(D7, i);delay(t);
    analogWrite(D8, i);delay(t);
    i = 0;
    analogWrite(D8, i);delay(t);
    analogWrite(D7, i);delay(t);
    analogWrite(D6, i);delay(t);
    analogWrite(D5, i);delay(t);
    analogWrite(D4, i);delay(t);
    analogWrite(D3, i);delay(t);
    analogWrite(D2, i);delay(t);
    analogWrite(D1, i);delay(t);
    analogWrite(D0, i);delay(t);



    i = 1023;
    t = random(10,200);
    analogWrite(D8, i);delay(t);
    analogWrite(D7, i);delay(t);
    analogWrite(D6, i);delay(t);
    analogWrite(D5, i);delay(t);
    analogWrite(D4, i);delay(t);
    analogWrite(D3, i);delay(t);
    analogWrite(D2, i);delay(t);
    analogWrite(D1, i);delay(t);
    analogWrite(D0, i);delay(t);
    i = 0;
    analogWrite(D0, i);delay(t);
    analogWrite(D1, i);delay(t);
    analogWrite(D2, i);delay(t);
    analogWrite(D3, i);delay(t);
    analogWrite(D4, i);delay(t);
    analogWrite(D5, i);delay(t);
    analogWrite(D6, i);delay(t);
    analogWrite(D7, i);delay(t);
    analogWrite(D8, i);delay(t);


    // digitalWrite(D0,random(0,1));delay(200);
    // digitalWrite(D1,random(0,1));delay(200);
    // digitalWrite(D2,random(0,1));delay(200);
    // digitalWrite(D3,random(0,1));delay(200);
    // digitalWrite(D4,random(0,1));delay(200);
    // digitalWrite(D5,random(0,1));delay(200);
    // digitalWrite(D6,random(0,1));delay(200);
    // digitalWrite(D7,random(0,1));delay(200);
    // digitalWrite(D8,random(0,1));delay(200);
}