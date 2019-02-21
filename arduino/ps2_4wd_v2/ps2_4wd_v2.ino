/*
    ps2手柄控制4轮驱动小车

 * BY: Malu
 * https://malu.me
 * 2019.2.21

*/

#include <PS2X_lib.h> //for v1.6

#include "MRUN_lib.h"

// NODEMCU版本引脚
int leftMotor1 = D5; // 前后轮子
int leftMotor2 = D6;
int rightMotor1 = D7; // 左右轮子
int rightMotor2 = D8;
// 接收机引脚
#define PS2_DAT D1 //14
#define PS2_CMD D2 //15
#define PS2_CS  D3 //16
#define PS2_CLK D4 //17

// // D1版本引脚
// int leftMotor1 = D2; // 前后轮子
// int leftMotor2 = D3;
// int rightMotor1 = D4; // 左右轮子
// int rightMotor2 = D5;
// // 接收机引脚
// #define PS2_DAT D13 //14
// #define PS2_CMD D12 //15
// #define PS2_CS D11  //16
// #define PS2_CLK D10 //17

// // UNO版本引脚
// int leftMotor1 = 2; // 左边轮子
// int leftMotor2 = 3;
// int rightMotor1 = 4; // 右边轮子
// int rightMotor2 = 5;
// // 接收机引脚
// #define PS2_DAT 13 //14
// #define PS2_CMD 11 //15
// #define PS2_CS 10  //16
// #define PS2_CLK 12 //17

// 驾驶定义
#define STOP 0
#define FORWARD 1
#define BACKWARD 2
#define TURNLEFT 3
#define TURNRIGHT 4

// 全局驾驶变量
int RUNCMD = 0;

/******************************************************************
   select modes of PS2 controller:
     - pressures = analog reading of push-butttons
     - rumble    = motor rumbling
   uncomment 1 of the lines for each mode selection
 ******************************************************************/
#define pressures false
#define rumble false

PS2X ps2x; // create PS2 Controller Class
MRUN mrun;

byte type = 0;
byte vibrate = 0;

// analogWrite(pin, value)  UNO:0-255  D1 ESP8266:0-1023
#define MAX_SPEED 1023 // PWM最大数值

void Mrun(int vl = 0, int vr = 0)
{
    Mrun_one(vl, leftMotor1, leftMotor2);
    Mrun_one(vr, rightMotor1, rightMotor2);
    // Serial.printf(" L:%d R:%d \n", vl, vr);
    Serial.print(" L:");
    Serial.print(vl);
    Serial.print(" R:");
    Serial.print(vr);
}

void Mrun_one(int v, int M1, int M2)
{
    int v_abs = abs(v);
    //   v_abs = v_abs<MAX_SPEED?v_abs:MAX_SPEED;
    v_abs = constrain(v_abs, 0, MAX_SPEED);
    if (v > 0)
    {
        digitalWrite(M1, LOW);
        analogWrite(M2, v_abs);
        // Serial.print(v_abs);
    }
    else if (v < 0)
    {
        digitalWrite(M1, HIGH);
        analogWrite(M2, MAX_SPEED - v_abs);
        // Serial.print(-v_abs);
    }
    else
    {
        digitalWrite(M1, LOW);
        digitalWrite(M2, LOW);
        // Serial.print(" 0 ");
    }
}

void Run(int cmd)
{
    switch (cmd)
    {
    case FORWARD:
        Serial.print(" FORWARD"); // 前
        Mrun(MAX_SPEED, MAX_SPEED);
        break;
    case BACKWARD:
        Serial.println("BACKWARD"); // 后
        Mrun(-MAX_SPEED, -MAX_SPEED);
        break;
    case TURNLEFT:
        Serial.println("TURNLEFT"); // 左
        Mrun(-MAX_SPEED, MAX_SPEED);
        break;
    case TURNRIGHT:
        Serial.println("TURNRIGHT"); // 右
        Mrun(MAX_SPEED, -MAX_SPEED);
        break;
    default:
        Serial.print("."); // 停止
        Mrun();
        break;
        // delay(50);
    }
}

void setup()
{
    Serial.begin(115200);

    int error = 0;
    do
    {
        error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_CS, PS2_DAT, pressures, rumble);
        if (error == 0)
        {
            break;
        }
        else
        {
            delay(100);
        }
    } while (1);

    // type = ps2x.readType();
    // switch (type)
    // {
    // case 0:
    //     Serial.println("Unknown Controller type");
    //     break;
    // case 1:
    //     Serial.println("DualShock Controller Found");
    //     break;
    // case 2:
    //     Serial.println("GuitarHero Controller Found");
    //     break;
    // }
}

void loop()
{
    ps2x.read_gamepad(false, vibrate); //read controller and set large motor to spin at 'vibrate' speed

    if (ps2x.Button(PSB_START)) //will be TRUE as long as button is pressed
        Serial.println("Start is being held");
    if (ps2x.Button(PSB_SELECT))
        Serial.println("Select is being held");









  
    // ps2x.read_gamepad(false, 0);          //read controller and set large motor to spin at 'vibrate' speed
    
    if(ps2x.Button(PSB_START))                   //will be TRUE as long as button is pressed
         Serial.println("Start is being held");
    if(ps2x.Button(PSB_SELECT))
         Serial.println("Select is being held");
         
         
     if(ps2x.Button(PSB_PAD_UP)) {         //will be TRUE as long as button is pressed
       Serial.print("Up held this hard: ");
       Serial.println(ps2x.Analog(PSAB_PAD_UP), DEC);
      }
      if(ps2x.Button(PSB_PAD_RIGHT)){
       Serial.print("Right held this hard: ");
        Serial.println(ps2x.Analog(PSAB_PAD_RIGHT), DEC);
      }
      if(ps2x.Button(PSB_PAD_LEFT)){
       Serial.print("LEFT held this hard: ");
        Serial.println(ps2x.Analog(PSAB_PAD_LEFT), DEC);
      }
      if(ps2x.Button(PSB_PAD_DOWN)){
       Serial.print("DOWN held this hard: ");
     Serial.println(ps2x.Analog(PSAB_PAD_DOWN), DEC);
      }   
  
    
    //   vibrate = ps2x.Analog(PSAB_BLUE);        //this will set the large motor vibrate speed based on 
                                              //how hard you press the blue (X) button    
    
    if (ps2x.NewButtonState())               //will be TRUE if any button changes state (on to off, or off to on)
    {
     
       
         
        if(ps2x.Button(PSB_L3))
         Serial.println("L3 pressed");
        if(ps2x.Button(PSB_R3))
         Serial.println("R3 pressed");
        if(ps2x.Button(PSB_L2))
         Serial.println("L2 pressed");
        if(ps2x.Button(PSB_R2))
         Serial.println("R2 pressed");
        if(ps2x.Button(PSB_GREEN))
         Serial.println("Triangle pressed");
         
    }   
         
    
    if(ps2x.ButtonPressed(PSB_RED))             //will be TRUE if button was JUST pressed
         Serial.println("Circle just pressed");
         
    if(ps2x.ButtonReleased(PSB_PINK))             //will be TRUE if button was JUST released
         Serial.println("Square just released");     
    
    if(ps2x.NewButtonState(PSB_BLUE))            //will be TRUE if button was JUST pressed OR released
         Serial.println("X just changed");    
    
    
    if(ps2x.Button(PSB_L1) || ps2x.Button(PSB_R1)) // print stick values if either is TRUE
    {
        Serial.print("Stick Values:");
        Serial.print(ps2x.Analog(PSS_LY), DEC); //Left stick, Y axis. Other options: LX, RY, RX  
        Serial.print(",");
        Serial.print(ps2x.Analog(PSS_LX), DEC); 
        Serial.print(",");
        Serial.print(ps2x.Analog(PSS_RY), DEC); 
        Serial.print(",");
        Serial.println(ps2x.Analog(PSS_RX), DEC); 
    } 





delay(50);
return;
    if (ps2x.Button(PSB_PAD_UP))
    { //will be TRUE as long as button is pressed
        Serial.println("Up held this hard: ");
        RUNCMD = FORWARD;
    }
    else if (ps2x.ButtonReleased(PSB_PAD_UP))
    {
        Serial.println("Up Button Released!");
        RUNCMD = STOP;
    }

    if (ps2x.Button(PSB_PAD_DOWN))
    {
        Serial.println("DOWN held this hard: ");
        RUNCMD = BACKWARD;
    }
    else if (ps2x.ButtonReleased(PSB_PAD_DOWN))
    {
        Serial.println("DOWN Button Released!");
        RUNCMD = STOP;
    }

    if (ps2x.Button(PSB_PAD_RIGHT))
    {
        Serial.println("Right held this hard: ");
        RUNCMD = TURNRIGHT;
    }
    else if (ps2x.ButtonReleased(PSB_PAD_RIGHT))
    {
        Serial.println("Right Button Released!");
        RUNCMD = STOP;
    }

    if (ps2x.Button(PSB_PAD_LEFT))
    {
        Serial.println("LEFT held this hard: ");
        RUNCMD = TURNLEFT;
    }
    else if (ps2x.ButtonReleased(PSB_PAD_LEFT))
    {
        Serial.println("LEFT Button Released!");
        RUNCMD = STOP;
    }

    // 右边按键群
    if (ps2x.Button(PSB_TRIANGLE))
    { //will be TRUE as long as button is pressed
        Serial.println("Triangle pressed");
        RUNCMD = FORWARD;
    }
    else if (ps2x.ButtonReleased(PSB_TRIANGLE))
    {
        Serial.println("Triangle Button Released!");
        RUNCMD = STOP;
    }

    if (ps2x.Button(PSB_CROSS))
    {
        Serial.println("PSB_CROSS pressed");
        RUNCMD = BACKWARD;
    }
    else if (ps2x.ButtonReleased(PSB_CROSS))
    {
        Serial.println("PSB_CROSS Button Released!");
        RUNCMD = STOP;
    }

    if (ps2x.Button(PSB_CIRCLE)) // 右
    {
        Serial.println("PSB_CIRCLE pressed");
        RUNCMD = TURNRIGHT;
    }
    else if (ps2x.ButtonReleased(PSB_CIRCLE))
    {
        Serial.println("PSB_CIRCLE Button Released!");
        RUNCMD = STOP;
    }

    if (ps2x.Button(PSB_SQUARE)) // 左
    {
        Serial.println("PSB_SQUARE pressed");
        RUNCMD = TURNLEFT;
    }
    else if (ps2x.ButtonReleased(PSB_SQUARE))
    {
        Serial.println("PSB_SQUARE Button Released!");
        RUNCMD = STOP;
    }

    if (ps2x.Button(PSB_L2))
        Serial.println("L2 pressed");
    if (ps2x.Button(PSB_R2))
        Serial.println("R2 pressed");

    if (ps2x.Button(PSB_L3))
        Serial.println("L3 pressed");
    if (ps2x.Button(PSB_R3))
        Serial.println("R3 pressed");

    if (ps2x.Button(PSB_L1) || ps2x.Button(PSB_R1))
    { //print stick values if either is TRUE
        Serial.print("Stick Values:");
        Serial.print(ps2x.Analog(PSS_LY), DEC); //Left stick, Y axis. Other options: LX, RY, RX
        Serial.print(",");
        Serial.print(ps2x.Analog(PSS_LX), DEC);
        Serial.print(",");
        Serial.print(ps2x.Analog(PSS_RY), DEC);
        Serial.print(",");
        Serial.println(ps2x.Analog(PSS_RX), DEC);
    }
    RUNCMD = FORWARD;
    Run(RUNCMD);
    mrun.test(4);
    Serial.println();
    delay(1000);
}
