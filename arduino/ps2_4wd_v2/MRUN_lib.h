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
  void config(int, int, int, int);
  void one(int, int, int);
  void one_l(int);
  void one_r(int);
  void two(int a=0,int b=0);
  void car(int y=0,int x=0);

private:
  int _m1;
  int _m2;
  int _m3;
  int _m4;
};

#endif