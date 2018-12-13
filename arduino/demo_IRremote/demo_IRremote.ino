#include <IRremote.h>
 
int PIN_RECV = 9;
IRrecv irrecv(PIN_RECV);
decode_results results;
 
void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn();
}
 
void loop() {
  if (irrecv.decode(&results)) {
    Serial.println(results.value);
    irrecv.resume();
  }
}
