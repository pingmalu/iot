// 定义引脚编号
const int trigPin = 13; //D4
const int echoPin = 14; //D3

// 定义变量
long duration;
int distance;

void setup()
{
    pinMode(trigPin, OUTPUT); // 将trigPin设置为输出
    pinMode(echoPin, INPUT);  // 将echoPin设置为输入
    Serial.begin(115200);       // 启动串行通信
}

void loop()
{
    // Clears the trigPin
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);

    //将trigPin设置为HIGH状态10微秒
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // 读取echoPin，以微秒为单位返回声波传播时间
    duration = pulseIn(echoPin, HIGH);

    // 计算距离
    distance = duration * 0.034 / 2;
    // 打印距离在串行监视器
    Serial.print("Distance: ");
    Serial.println(distance);
    delay(10);
}