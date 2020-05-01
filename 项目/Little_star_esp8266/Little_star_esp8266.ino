
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
    for (int i = 0; i < 1023; i++)
    {
        analogWrite(D0, i);delay(random(1,4));
        analogWrite(D1, i);delay(random(1,4));
        analogWrite(D2, i);delay(random(1,4));
        analogWrite(D3, i);delay(random(1,4));
        analogWrite(D4, i);delay(random(1,4));
        analogWrite(D5, i);delay(random(1,4));
        analogWrite(D6, i);delay(random(1,4));
        analogWrite(D7, i);delay(random(1,4));
        analogWrite(D8, i);delay(random(1,4));
    }
    for (int i = 1023; i > 0; i--)
    {
        analogWrite(D0, i);delay(random(1,4));
        analogWrite(D1, i);delay(random(1,4));
        analogWrite(D2, i);delay(random(1,4));
        analogWrite(D3, i);delay(random(1,4));
        analogWrite(D4, i);delay(random(1,4));
        analogWrite(D5, i);delay(random(1,4));
        analogWrite(D6, i);delay(random(1,4));
        analogWrite(D7, i);delay(random(1,4));
        analogWrite(D8, i);delay(random(1,4));
    }
}