////////////////////////////--------------------Libraries-------------------------------------------/////////////////////////////////////
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
// #include <Wire.h>
#include <Servo.h> // 舵机控制
Servo SV1;         // 创建舵机对象
Servo SV2;         // 创建舵机对象
////////////////////////////--------------------Variables-------------------------------------------/////////////////////////////////////
WiFiUDP Udp;
const int LED = LED_BUILTIN;
const int Y_PIN = D1;
const int X_PIN = D2;
////////////////////////////--------------------Don't change these Settings-------------------------------------------/////////////////////////////////////
/* Don't change this stuff(because the phone will be looking for us based on SSID name and port) */ //Note, we are not connecting to WiFi, because we are looking to optimize speed.  You can connect to WiFi if you want
const char *ssid = "ESPTest";
const char *password = "";
unsigned int localUdpPort = 4210; // local port to listen on

////////////////////////////--------------------SETUP-------------------------------------------/////////////////////////////////////

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  Serial.print("Port: ");
  Serial.println(localUdpPort);

  Udp.begin(localUdpPort);

  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);

  // 舵机引脚初始化
  SV1.attach(Y_PIN);
  SV2.attach(X_PIN);
}

////////////////////////////--------------------LOOP-------------------------------------------/////////////////////////////////////

void loop()
{
  // put your main code here, to run repeatedly:
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    char InputType, InputValue;
    //When we get our message, we need to evaluate the message type(Is it a button? Is it a joystick?), then we need to get its value.
    while (!isAlpha(InputType = Udp.read()))
      ; //Keep skipping until we get a letter.
    while (!isdigit(InputValue = Udp.read()))
      ; //Keep skipping until we get a integer.

    int num = InputValue - '0';
    while (isdigit(InputValue = Udp.read()))
    {
      num = (num * 10) + (InputValue - '0'); //some magic
    }
    useGameData(InputType, num);
  }
}
////////////////////////////--------------------Process Controller data-------------------------------------------/////////////////////////////////////

void useGameData(char type, int value)
{
  yield();

  int num;
  switch (type)
  {
  case 'a':
    switch (value)
    {
    case 1:
      digitalWrite(LED, LOW);
      break;
    case 0:
      digitalWrite(LED, HIGH);
      break;
    }
    break;

  //left joystick Y direction
  case 'l':
    //this will bring it down to a range 0-255;
    num = map(value, 0, 255, 0, 180);
    SV1.write(num);
    break;

  //Right joystick X direction
  case 'z':
    //this will bring it down to a range 0-255;
    num = map(value, 0, 255, 0, 180);
    SV2.write(num);
    break;
  }
  yield();
}
