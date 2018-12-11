/**
 * L298N模块，调速测试
 */

int IN1 = 4;
int IN2 = 5;
int IN3 = 6;
int IN4 = 7;
                       

void setup() { 
   pinMode(IN2, OUTPUT);
   pinMode(IN4, OUTPUT);
}

void loop() { 
  int value;

  for(value=0;value<=255;value+=5){
    digitalWrite(IN2,HIGH);   
    digitalWrite(IN4, HIGH);       
    analogWrite(IN1, value);   //PWM调速
    analogWrite(IN3, value);   //PWM调速
    delay(30);
  }
}
