////////////////////////////--------------------Libraries-------------------------------------------/////////////////////////////////////
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
// #include <Wire.h>
#include <Servo.h> // 舵机控制
Servo SV1;         // 创建舵机对象
Servo SV2;         // 创建舵机对象
Servo SV3;         // 创建舵机对象
Servo SV4;         // 创建舵机对象
////////////////////////////--------------------Variables-------------------------------------------/////////////////////////////////////
WiFiUDP Udp;
const int LED = LED_BUILTIN;
const int Y_PIN = D1;
const int X_PIN = D2;
const int Y_PIN_R = D3;
const int X_PIN_R = D4;
const int LED_2 = D8;
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
  pinMode(LED_2, OUTPUT);
  digitalWrite(LED_2, LOW);

  // 舵机引脚初始化
  SV1.attach(Y_PIN);
  SV2.attach(X_PIN);
  SV3.attach(Y_PIN_R);
  SV4.attach(X_PIN_R);
}

bool led2 = false;
void led2f()
{
  if (led2)
  {
    digitalWrite(LED_2, HIGH);
  }
  else
  {
    digitalWrite(LED_2, LOW);
  }
  led2 = led2 ? false : true;
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

  Serial.print(type);
  Serial.print("=");
  Serial.print(value);
  Serial.println();

  int num;
  switch (type)
  {
  case 'a':
    switch (value)
    {
    case 1:
      digitalWrite(LED, LOW);
      Serial.print("led2 low");
      break;
    case 0:
      digitalWrite(LED, HIGH);
      Serial.print("led2 high");
      break;
    }
    break;

  //left joystick X direction
  case 'k':
    //this will bring it down to a range 0-255;
    num = map(value, 100, 355, 180, 0);
    SV1.write(num);
    break;

  //left joystick X direction
  case 'l':
    //this will bring it down to a range 0-255;
    num = map(value, 100, 355, 180, 0);
    SV2.write(num);
    break;
  //Right joystick X direction
  case 'z':
    num = map(value, 100, 355, 120, 60);
    SV3.write(num);
    break;
  //Right joystick Y direction
  case 'x':
    num = map(value, 100, 355, 120, 60);
    SV4.write(num);
    break;

    //-------------Dpad Buttons------------//
  case 'd':
    switch (value)
    {
      //Dpad Center Button
    case 0:
      led2f();
      // Serial.println("D-pad Center");
      break;
    //Dpad Left Button
    case 1:
      // Serial.println("D-pad Left");
      break;
    //Dpad Up Button
    case 2:
      // Serial.println("D-pad Up");
      break;
    //Dpad Right Button
    case 3:
      // Serial.println("D-pad Right");
      break;
    //Dpad Down Button
    case 4:
      // Serial.println("D-pad Down");
      break;
    }
    break;
  }
  yield();
}
