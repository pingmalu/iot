/**
 * 
 * 
 */
#if ARDUINO > 22
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

class MRUN
{
  public:
    void test(uint16_t);
    void one(int,int,int);
};