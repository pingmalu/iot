int val;

void setup() 
{
  Serial.begin(115200);
}
 
void loop()
{
    val = analogRead(A0);
    Serial.println(val);
    delay(100);
}