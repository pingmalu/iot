/**
 * 
 * 
 */
#ifndef MRUN_lib_h
#define MRUN_lib_h

#if ARDUINO > 22
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class MRUN
{
public:
  void config(int m1=0, int m2=0, int m3=0, int m4=0, int Y_MAX = 0, int Y_MID = 0, int Y_MIN = 0, int X_MAX = 0, int X_MID = 0, int X_MIN = 0, int SILL = 0);
  void one(int, int, int);
  void one_l(int);
  void one_r(int);
  void two(int a=0,int b=0);
  void car(int y=0,int x=0);
  void tank(int y=0,int x=0);
  void tank_v2(int y=0,int x=0);

private:
  int _m1;
  int _m2;
  int _m3;
  int _m4;

  int _Y_MAX;
  int _Y_MID;
  int _Y_MIN;
  int _X_MAX;
  int _X_MID;
  int _X_MIN;
  int _SILL;    // 偏移阈值

  int _RUN_SPEED;  // 电机前进值
  int _LR;  // 电机左右值
  int _tmp_RUN_SPEED;  // 前后粘滞值
  // int _tmp_LR;  // 左右粘滞值
};

#endif