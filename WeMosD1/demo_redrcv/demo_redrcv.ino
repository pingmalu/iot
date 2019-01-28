// 可调距离，接收到反射0，无接收（黑线）1
// 不可调节距离，hw-006 v1.2,接收到反射1,无接收（黑线）0

int PIN_SENSOR = 0;  //wemos D1: D8-0
int PIN_SENSOR_A0 = A0;

 
void setup() {
  pinMode(PIN_SENSOR, INPUT);
  pinMode(PIN_SENSOR_A0, INPUT);
  Serial.begin(9600);
}
 
void loop() {
  int x = digitalRead(PIN_SENSOR);
  int a = analogRead(PIN_SENSOR_A0);
  Serial.print(a);
  Serial.println(x);
  delay(100);
}
