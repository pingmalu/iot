/**
 * 
 * 
 */
#include "MRUN_lib.h"

#if ARDUINO > 22
#include "Arduino.h"
#else
#include "WProgram.h"
#include "pins_arduino.h"
#endif

#ifndef ESP8266
#define MAX_SPEED 255
#define START_SPEED 50
#define LR_MIN_SPEED 200 // 粘滞转向最低推进速度
#else
#define MAX_SPEED 1023
#define START_SPEED 200
#define LR_MIN_SPEED 800 // 粘滞转向最低推进速度
#endif

void MRUN::config(int m1, int m2, int m3, int m4, int Y_MAX, int Y_MID, int Y_MIN, int X_MAX, int X_MID, int X_MIN, int SILL)
{
  _m1 = m1;
  _m2 = m2;
  _m3 = m3;
  _m4 = m4;

  _Y_MAX = Y_MAX;
  _Y_MID = Y_MID;
  _Y_MIN = Y_MIN;
  _X_MAX = X_MAX;
  _X_MID = X_MID;
  _X_MIN = X_MIN;
  _SILL = SILL;
  Serial.println(" config OK");
}

void MRUN::one(int v, int M1, int M2)
{
  int v_abs = abs(v);
  v_abs = constrain(v_abs, 0, MAX_SPEED);
  if (v > 0)
  {
    digitalWrite(M1, LOW);
    analogWrite(M2, v_abs);
    Serial.print(" one:");
    Serial.print(v_abs);
  }
  else if (v < 0)
  {
    digitalWrite(M1, HIGH);
    analogWrite(M2, MAX_SPEED - v_abs);
    Serial.print(" one:");
    Serial.print(-v_abs);
  }
  else
  {
    digitalWrite(M1, LOW);
    digitalWrite(M2, LOW);
    // Serial.print(" 0 ");
  }
}

void MRUN::one_l(int v)
{
  one(v, _m1, _m2);
}

void MRUN::one_r(int v)
{
  one(v, _m3, _m4);
}

void MRUN::two(int vl, int vr)
{
  one(vl, _m1, _m2);
  one(vr, _m3, _m4);
  // Serial.printf(" L:%d R:%d \n", vl, vr);
  Serial.print(" two_L:");
  Serial.print(vl);
  Serial.print(" tow_R:");
  Serial.print(vr);
}

void MRUN::car(int y, int x)
{
  // one(y, _m1, _m2);
  // one(x, _m3, _m4);
  // Serial.printf(" L:%d R:%d \n", vl, vr);
  // Serial.print(" ");
  // Serial.print(_Y_MAX);
  // Serial.print(_Y_MID);
  // Serial.print(_Y_MIN);
  // Serial.print(_X_MAX);
  // Serial.print(_X_MID);
  // Serial.print(_X_MIN);
  // Serial.print(_SILL);

  Serial.print(" car_L:");
  Serial.print(y);
  Serial.print(" car_R:");
  Serial.print(x);

  if (_Y_MAX == 0 && _Y_MIN == 0 && _X_MAX == 0 && _X_MIN == 0)
  {
    one(y, _m1, _m2);
    one(x, _m3, _m4);
    return;
  }

  y = constrain(y, _Y_MIN, _Y_MAX);
  x = constrain(x, _X_MIN, _X_MAX);

  if (x > (_X_MID + _SILL)) // 右拉 right >
  {
    _LR = map(x, _X_MID + _SILL, _X_MAX, START_SPEED, MAX_SPEED);
  }
  else if (x < (_X_MID - _SILL)) // 左拉 left <
  {
    _LR = map(x, _X_MID - _SILL, _X_MIN, -START_SPEED, -MAX_SPEED);
  }
  else
  { // 回中
    _LR = 0;
  }

  if (_LR == 0) // 粘滞算法用
  {
    if (y > (_Y_MID + _SILL)) // 上拉 up ^
    {
      _RUN_SPEED = map(y, _Y_MID + _SILL, _Y_MAX, START_SPEED, MAX_SPEED);
    }
    else if (y < (_Y_MID - _SILL)) // 下拉 down v
    {
      _RUN_SPEED = map(y, _Y_MID - _SILL, _Y_MIN, -START_SPEED, -MAX_SPEED);
    }
    else
    { // 回中
      _RUN_SPEED = 0;
    }
  }

  // 粘滞算法
  if (_LR != 0)
  {
    if (_RUN_SPEED > 0)
    { // 前
      // _RUN_SPEED = constrain(_RUN_SPEED, LR_MIN_SPEED, MAX_SPEED);  // 高速转向
      _RUN_SPEED = constrain(_RUN_SPEED, START_SPEED, LR_MIN_SPEED); // 低速转向
    }
    else if (_RUN_SPEED < 0)
    { // 后
      // _RUN_SPEED = constrain(_RUN_SPEED, -MAX_SPEED, -LR_MIN_SPEED);  // 高速转向
      _RUN_SPEED = constrain(_RUN_SPEED, -LR_MIN_SPEED, -START_SPEED); // 低速转向
    }
    else // 直接左右拉，再切换上下
    {
      if (y > (_Y_MID + _SILL)) // 上拉 up ^
      {
        _RUN_SPEED = map(y, _Y_MID + _SILL, _Y_MAX, START_SPEED, MAX_SPEED);
      }
      else if (y < (_Y_MID - _SILL)) // 下拉 down v
      {
        _RUN_SPEED = map(y, _Y_MID - _SILL, _Y_MIN, -START_SPEED, -MAX_SPEED);
      }
      else
      { // 回中
        _RUN_SPEED = 0;
      }
    }
  }

  two(_RUN_SPEED, _LR);
}

void MRUN::tank(int y, int x)
{
  // one(y, _m1, _m2);
  // one(x, _m3, _m4);
  // Serial.printf(" L:%d R:%d \n", vl, vr);
  // Serial.print(" ");
  // Serial.print(_Y_MAX);
  // Serial.print(_Y_MID);
  // Serial.print(_Y_MIN);
  // Serial.print(_X_MAX);
  // Serial.print(_X_MID);
  // Serial.print(_X_MIN);
  // Serial.print(_SILL);

  Serial.print(" tank_L:");
  Serial.print(y);
  Serial.print(" tank_R:");
  Serial.print(x);

  if (_Y_MAX == 0 && _Y_MIN == 0 && _X_MAX == 0 && _X_MIN == 0)
  {
    one(y, _m1, _m2);
    one(x, _m3, _m4);
    return;
  }

  y = constrain(y, _Y_MIN, _Y_MAX);
  x = constrain(x, _X_MIN, _X_MAX);

  if (x > (_X_MID + _SILL)) // 右拉 right >
  {
    _LR = map(x, _X_MID + _SILL, _X_MAX, START_SPEED, MAX_SPEED);
  }
  else if (x < (_X_MID - _SILL)) // 左拉 left <
  {
    _LR = map(x, _X_MID - _SILL, _X_MIN, -START_SPEED, -MAX_SPEED);
  }
  else
  { // 回中
    _LR = 0;
  }

  if (_LR == 0) // 粘滞算法用
  {
    if (y > (_Y_MID + _SILL)) // 上拉 up ^
    {
      _RUN_SPEED = map(y, _Y_MID + _SILL, _Y_MAX, START_SPEED, MAX_SPEED);
    }
    else if (y < (_Y_MID - _SILL)) // 下拉 down v
    {
      _RUN_SPEED = map(y, _Y_MID - _SILL, _Y_MIN, -START_SPEED, -MAX_SPEED);
    }
    else
    { // 回中
      _RUN_SPEED = 0;
    }
  }

  // 粘滞算法
  if (_LR != 0)
  {
    if (_RUN_SPEED > 0)
    { // 前
      // _RUN_SPEED = constrain(_RUN_SPEED, LR_MIN_SPEED, MAX_SPEED);  // 高速转向
      _RUN_SPEED = constrain(_RUN_SPEED, START_SPEED, LR_MIN_SPEED); // 低速转向
    }
    else if (_RUN_SPEED < 0)
    { // 后
      // _RUN_SPEED = constrain(_RUN_SPEED, -MAX_SPEED, -LR_MIN_SPEED);  // 高速转向
      _RUN_SPEED = constrain(_RUN_SPEED, -LR_MIN_SPEED, -START_SPEED); // 低速转向
    }
    else // 直接左右拉，再切换上下
    {
      if (y > (_Y_MID + _SILL)) // 上拉 up ^
      {
        _RUN_SPEED = map(y, _Y_MID + _SILL, _Y_MAX, START_SPEED, MAX_SPEED);
      }
      else if (y < (_Y_MID - _SILL)) // 下拉 down v
      {
        _RUN_SPEED = map(y, _Y_MID - _SILL, _Y_MIN, -START_SPEED, -MAX_SPEED);
      }
      else
      { // 回中
        _RUN_SPEED = 0;
      }
    }
  }

  if (_RUN_SPEED == 0 && _LR != 0)
  { // 原地左右
    two(_LR, -_LR);
  }
  else
  {
    if (_LR == 0)
    {
      two(_RUN_SPEED, _RUN_SPEED); // 原地前进后退
    }
    else if (_RUN_SPEED > 0)
    { // 前
      if (_LR >= 0)
      {
        two(_RUN_SPEED + _LR, _RUN_SPEED);
      }
      else
      {
        two(_RUN_SPEED, _RUN_SPEED - _LR);
      }
    }
    else
    { // 后
      if (_LR >= 0)
      {
        two(_RUN_SPEED - _LR, _RUN_SPEED);
      }
      else
      {
        two(_RUN_SPEED, _RUN_SPEED + _LR);
      }
    }
  }

}