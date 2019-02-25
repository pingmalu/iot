/*
 ps2手柄中继控制器

 * BY: Malu
 * https://malu.me
 * 2019.2.25

*/

#include <PS2X_lib.h>

// UNO版本引脚
#define up 2
#define down 3
#define left 4
#define right 5
// 接收机引脚
#define PS2_DAT 13
#define PS2_CMD 12
#define PS2_CS 11
#define PS2_CLK 10

// // NODEMCU版本引脚
// #define up D5
// #define down D6
// #define left D7
// #define right D8
// // 接收机引脚
// #define PS2_DAT D1
// #define PS2_CMD D2
// #define PS2_CS  D3
// #define PS2_CLK D4

// // D1版本引脚
// #define up D2
// #define down D3
// #define left D4
// #define right D5
// // 接收机引脚
// #define PS2_DAT D13
// #define PS2_CMD D12
// #define PS2_CS  D11
// #define PS2_CLK D10

/******************************************************************
   select modes of PS2 controller:
     - pressures = analog reading of push-butttons
     - rumble    = motor rumbling
   uncomment 1 of the lines for each mode selection
 ******************************************************************/
#define pressures false
#define rumble false

PS2X ps2x; // create PS2 Controller Class

int R_RUN_SPEED = 0; // 推进速度
int R_LR = 0;        // 转向速度
int L_RUN_SPEED = 0; // 推进速度
int L_LR = 0;        // 转向速度

#define Y_MID 128
#define SILL 64

bool up_down_tag = false;
bool left_right_tag = false;

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
        else if (error == 1)
        {
            Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");
        }
        else if (error == 2)
        {
            Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");
        }
        else if (error == 3)
        {
            Serial.println("Controller refusing to enter Pressures mode, may not support it. ");
        }
        else
        {
            Serial.println("Unknown error");
        }
        delay(100);
    } while (1);

    // 电机驱动引脚初始化
    pinMode(up, OUTPUT);
    pinMode(down, OUTPUT);
    pinMode(left, OUTPUT);
    pinMode(right, OUTPUT);
}

void loop()
{

    ps2x.read_gamepad(false, 0); //read controller and set large motor to spin at 'vibrate' speed

    // 左边按键群
    if (ps2x.Button(PSB_PAD_UP))
    { //will be TRUE as long as button is pressed
        Serial.println("Up held this hard: ");
        digitalWrite(up, LOW);
    }
    else if (ps2x.ButtonReleased(PSB_PAD_UP))
    {
        Serial.println("Up Button Released!");
        digitalWrite(up, HIGH);
    }

    if (ps2x.Button(PSB_PAD_DOWN))
    {
        Serial.println("DOWN held this hard: ");
        digitalWrite(down, LOW);
    }
    else if (ps2x.ButtonReleased(PSB_PAD_DOWN))
    {
        Serial.println("DOWN Button Released!");
        digitalWrite(down, HIGH);
    }

    if (ps2x.Button(PSB_PAD_RIGHT))
    {
        Serial.println("Right held this hard: ");
        digitalWrite(right, LOW);
    }
    else if (ps2x.ButtonReleased(PSB_PAD_RIGHT))
    {
        Serial.println("Right Button Released!");
        digitalWrite(right, HIGH);
    }

    if (ps2x.Button(PSB_PAD_LEFT))
    {
        Serial.println("LEFT held this hard: ");
        digitalWrite(left, LOW);
    }
    else if (ps2x.ButtonReleased(PSB_PAD_LEFT))
    {
        Serial.println("LEFT Button Released!");
        digitalWrite(left, HIGH);
    }

    // 右边按键群
    if (ps2x.Button(PSB_TRIANGLE)) // 上 三角
    {                              //will be TRUE as long as button is pressed
        Serial.println(" ▲ Triangle pressed");
        digitalWrite(up, LOW);
    }
    else if (ps2x.ButtonReleased(PSB_TRIANGLE))
    {
        Serial.println(" ▲ Triangle Button Released!");
        digitalWrite(up, HIGH);
    }

    if (ps2x.Button(PSB_CROSS)) // 下 叉
    {
        Serial.println(" ✗ PSB_CROSS pressed");
        digitalWrite(down, LOW);
    }
    else if (ps2x.ButtonReleased(PSB_CROSS))
    {
        Serial.println(" ✗ PSB_CROSS Button Released!");
        digitalWrite(down, HIGH);
    }

    if (ps2x.Button(PSB_CIRCLE)) // 右 圈
    {
        Serial.println(" ○ PSB_CIRCLE pressed");
        digitalWrite(right, LOW);
    }
    else if (ps2x.ButtonReleased(PSB_CIRCLE))
    {
        Serial.println(" ○ PSB_CIRCLE Button Released!");
        digitalWrite(right, HIGH);
    }

    if (ps2x.Button(PSB_SQUARE)) // 左 正方
    {
        Serial.println(" □ PSB_SQUARE pressed");
        digitalWrite(left, LOW);
    }
    else if (ps2x.ButtonReleased(PSB_SQUARE))
    {
        Serial.println(" □ PSB_SQUARE Button Released!");
        digitalWrite(left, HIGH);
    }

    // 摇杆控制器 start
    R_RUN_SPEED = map(constrain((int)ps2x.Analog(PSS_RY), 0, 255), 0, 255, 0, 255);
    R_LR = map(constrain((int)ps2x.Analog(PSS_RX), 0, 255), 0, 255, 0, 255);
    L_RUN_SPEED = map(constrain((int)ps2x.Analog(PSS_LY), 0, 255), 0, 255, 0, 255);
    L_LR = map(constrain((int)ps2x.Analog(PSS_LX), 0, 255), 0, 255, 0, 255);

    if ((Y_MID + SILL) < R_RUN_SPEED || (Y_MID + SILL) < L_RUN_SPEED) // 上
    {
        up_down_tag = true;
        digitalWrite(up, LOW);
    }
    else if ((Y_MID - SILL) > R_RUN_SPEED || (Y_MID - SILL) > L_RUN_SPEED) // 下
    {
        up_down_tag = true;
        digitalWrite(down, LOW);
    }
    else
    {
        if (up_down_tag)
        {
            digitalWrite(up, HIGH);
            digitalWrite(down, HIGH);
            up_down_tag = false;
        }
    }

    if ((Y_MID + SILL) < R_LR || (Y_MID + SILL) < L_LR) // 左
    {
        left_right_tag = true;
        digitalWrite(left, LOW);
    }
    else if ((Y_MID - SILL) > R_LR || (Y_MID - SILL) > L_LR) // 右
    {
        left_right_tag = true;
        digitalWrite(right, LOW);
    }
    else
    {
        if (left_right_tag)
        {
            digitalWrite(left, HIGH);
            digitalWrite(right, HIGH);
            left_right_tag = false;
        }
    }

    Serial.println();
    delay(10);
}
