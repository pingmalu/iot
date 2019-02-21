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

#define MAX_SPEED 1023

void MRUN::config(int m1, int m2, int m3, int m4)
{
  _m1 = m1;
  _m2 = m2;
  _m3 = m3;
  _m4 = m4;
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
    Serial.print(v_abs);
  }
  else if (v < 0)
  {
    digitalWrite(M1, HIGH);
    analogWrite(M2, MAX_SPEED - v_abs);
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
  Serial.print(" L:");
  Serial.print(vl);
  Serial.print(" R:");
  Serial.print(vr);
}

void MRUN::car(int y, int x)
{
  one(y, _m1, _m2);
  one(x, _m3, _m4);
  // Serial.printf(" L:%d R:%d \n", vl, vr);
  Serial.print(" L:");
  Serial.print(y);
  Serial.print(" R:");
  Serial.print(x);
}