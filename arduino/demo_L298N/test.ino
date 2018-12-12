/**
 * L298N模块，调速测试
 * 参考教程 https://blog.csdn.net/ace0perfect/article/details/79221628
 * https://blog.csdn.net/baidu_35679960/article/details/78961590
 * https://blog.csdn.net/ling3ye/article/details/51351115
 */

int IN1 = 4;
int IN2 = 5;
//int ENA = 6;

unsigned long time = 1200;  //delay time
int value = 150;   // the duty cycle 100-150


void setup() { 
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
 
    //set direction
//    digitalWrite(IN1, HIGH);
//    digitalWrite(IN2, LOW);
}

void loop() { 
//   analogWrite(ENA, value);
//delay(time);

  
//  int value;

  digitalWrite(IN1, HIGH);   
//  digitalWrite(IN2, LOW);   
  analogWrite(IN2, 200);   
////  digitalWrite(IN2, HIGH);
//  analogWrite(IN2, 50);
    delay(3000);
//
//  digitalWrite(IN1, LOW);   
//  digitalWrite(IN2, LOW);   
//    delay(3000);
//
    digitalWrite(IN1, LOW);   
  analogWrite(IN2, 50);
    delay(3000);

//  for(value=0;value<=100;value+=5){
//    digitalWrite(IN1, LOW);   
//    analogWrite(IN2, value);       
//    delay(130);
//  }
}
