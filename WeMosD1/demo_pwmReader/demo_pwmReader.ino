unsigned long duration;  //定义duration变量为无符号长整数型变量

int pwm_PIN1 = D2;

void setup()
{
  Serial.begin(9600);  //串口波特率为9600
  pinMode(pwm_PIN1, INPUT); //设置引脚为输入模式
} 

// pulseIn(pin, value, timeout)

void loop()
{
  duration = pulseIn(pwm_PIN1, 1,1000000); //读取引脚上的高电平脉冲，最大脉冲时间间隔为1秒，并且把结果赋值给duration变量 返回时间单位为微秒
  Serial.print(duration/1000000.0); //通过串口输出duration变量
  Serial.println(" s");
}
