unsigned long duration;  //定义duration变量为无符号长整数型变量

int pwm_PIN1 = D5;

void setup()
{
  Serial.begin(9600);  //串口波特率为9600
  pinMode(pwm_PIN1, INPUT); //设置引脚为输入模式
} 

// pulseIn(pin, value, timeout)
// 1左右 脉冲范围 中：1621  低：2096  高：1113  抖动：50
// 2上下 脉冲范围 中：1360  低：869  高：1860  抖动：50
// 3油门 低：884  高：1853
// 4油门左右  中：1600  低：1108  高：2096
// 5 下：1977  中：1480  上：989
// 6 下：1977  上：989
// D/R 舵量

void loop()
{
  duration = pulseIn(pwm_PIN1, HIGH); //读取引脚上的高电平脉冲，最大脉冲时间间隔为1秒，并且把结果赋值给duration变量 返回时间单位为微秒
  Serial.println(duration);
//  Serial.print(duration/1000000.0); //通过串口输出duration变量
//  Serial.println(" s");
}
