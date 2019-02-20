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

void MRUN::test(uint16_t button) {
  Serial.println("OUT:IN");
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