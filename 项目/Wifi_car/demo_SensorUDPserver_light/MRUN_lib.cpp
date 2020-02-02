/**
 * 
 * 
 */
#include "MRUN_lib.h"
// #include <Arduino.h>
// #include "pins_arduino.h"

// comment out this line, if you want to show logs:
// #define NDEBUG

#ifdef NDEBUG
#define LOG(...)
#define LOGLN(...)
#else
#define LOG(...) Serial.print(__VA_ARGS__)
#define LOGLN(...) Serial.println(__VA_ARGS__)
#endif

#define MAX_SPEED 1023
#define START_SPEED 80
#define LR_MIN_SPEED 800 // 粘滞转向最低推进速度

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
  MAX_RUN_SPEED = MAX_SPEED;
  EEP_UP_DWON_TAG = false;
  LOGLN(" config OK");
}

void MRUN::one(int v, int M1, int M2)
{
  int v_abs = abs(v);
  v_abs = constrain(v_abs, 0, MAX_RUN_SPEED);
  if (v_abs < START_SPEED) // 小于启动速度，不要发送PWM
  {
    digitalWrite(M1, LOW);
    digitalWrite(M2, LOW);
    return;
  }
  if (v > 0)
  {
    analogWrite(M1, LOW);
    analogWrite(M2, v_abs);
    LOG(" one:");
    LOG(v_abs);
  }
  else if (v < 0)
  {
    digitalWrite(M1, HIGH);
    analogWrite(M2, v_abs);
    LOG(" xne:");
    LOG(-v_abs);
  }
  else
  {
    digitalWrite(M1, LOW);
    digitalWrite(M2, LOW);
    // LOG(" 0 ");
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
  // LOGf(" L:%d R:%d \n", vl, vr);
  LOG(" two_L:");
  LOG(vl);
  LOG(" tow_R:");
  LOG(vr);
  LOGLN();
}

void MRUN::car(int y, int x)
{
  // one(y, _m1, _m2);
  // one(x, _m3, _m4);
  // LOGf(" L:%d R:%d \n", vl, vr);
  // LOG(" ");
  // LOG(_Y_MAX);
  // LOG(_Y_MID);
  // LOG(_Y_MIN);
  // LOG(_X_MAX);
  // LOG(_X_MID);
  // LOG(_X_MIN);
  // LOG(_SILL);

  LOG(" car_L:");
  LOG(y);
  LOG(" car_R:");
  LOG(x);

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
  // LOGf(" L:%d R:%d \n", vl, vr);
  // LOG(" ");
  // LOG(_Y_MAX);
  // LOG(_Y_MID);
  // LOG(_Y_MIN);
  // LOG(_X_MAX);
  // LOG(_X_MID);
  // LOG(_X_MIN);
  // LOG(_SILL);

  LOG(" tank_L:");
  LOG(y);
  LOG(" tank_R:");
  LOG(x);

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
    if (EEP_UP_DWON_TAG)
    {

      two(_LR, -_LR);
    }
    else
    {
      two(-_LR, _LR);
    }
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
        two(START_SPEED + _LR, START_SPEED);
      }
      else
      {
        two(START_SPEED, START_SPEED - _LR);
      }
    }
    else
    { // 后
      if (_LR >= 0)
      {
        two(-START_SPEED - _LR, -START_SPEED);
      }
      else
      {
        two(-START_SPEED, -START_SPEED + _LR);
      }
    }
  }
}

void MRUN::tank_v2(int y, int x)
{
  LOG(" tank_v2_1:");
  LOG(y);
  LOG(" tank_v2_2:");
  LOG(x);

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

  // 粘滞器
  if (_LR != 0 && _RUN_SPEED != 0)
  {
    if (_RUN_SPEED > 0)
    {
      if (_tmp_RUN_SPEED == 0)
      {
        _tmp_RUN_SPEED = _RUN_SPEED;
      }
    }
    else
    {
      if (_tmp_RUN_SPEED == 0)
      {
        _tmp_RUN_SPEED = _RUN_SPEED;
      }
    }
  }
  else if (_LR == 0 && _RUN_SPEED == 0)
  {
    _tmp_RUN_SPEED = 0;
  }

  if (_tmp_RUN_SPEED == 0)
  {
    if (_RUN_SPEED == 0 && _LR != 0) // 原地左右
    {
      if (EEP_UP_DWON_TAG)
      {
        two(_LR, -_LR);
      }
      else
      {
        two(-_LR, _LR);
      }
    }
    else if (_RUN_SPEED != 0 && _LR == 0) // 原地前后
    {
      two(_RUN_SPEED, _RUN_SPEED);
    }
    else // 都为0，停止
    {
      two(0, 0);
    }
  }
  else // 粘滞中
  {
    if (_tmp_RUN_SPEED > 0) // 前粘滞
    {
      if (_LR > 0) // 粘滞右
      {
        two(_tmp_RUN_SPEED, constrain(map(y, _Y_MIN, _Y_MAX, 0, MAX_SPEED), 0, _tmp_RUN_SPEED));
      }
      else if (_LR < 0) // 粘滞左
      {
        two(constrain(map(y, _Y_MIN, _Y_MAX, 0, MAX_SPEED), 0, _tmp_RUN_SPEED), _tmp_RUN_SPEED);
      }
    }
    else // 后粘滞
    {
      if (_LR > 0) // 粘滞右
      {
        two(_tmp_RUN_SPEED, -constrain(map(y, _Y_MIN, _Y_MAX, MAX_SPEED, 0), 0, -_tmp_RUN_SPEED));
      }
      else if (_LR < 0) // 粘滞左
      {
        two(-constrain(map(y, _Y_MIN, _Y_MAX, MAX_SPEED, 0), 0, -_tmp_RUN_SPEED), _tmp_RUN_SPEED);
      }
    }
  }
}