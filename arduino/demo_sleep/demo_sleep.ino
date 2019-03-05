#include <Enerlib.h>

Energy energy;             // 宣告"Energy"程式物件

const byte swPin = 2;      // 開關腳位
const byte ledPin = 13;    // LED腳位
byte times = 0;            // 記錄執行次數
volatile byte state = 0;   // 暫存執行狀態

void wakeISR() {
   if (energy.WasSleeping()) {
    state = 1;
  } else {
    state = 2;
  }
}

void setup() {
  Serial.begin(115200);
 
  pinMode(ledPin, OUTPUT);
  pinMode(swPin, INPUT);
  digitalWrite(swPin, HIGH);
 
  attachInterrupt(0, wakeISR, CHANGE);  // 附加中斷服務常式
  
  Serial.println("Running...");
}

void loop()
{
  if (state == 1) {
    Serial.println("Was sleeping...");
  } else if (state == 2) {
    Serial.println("Was awake...");
  }
  state = 0;
  
  digitalWrite(ledPin, !digitalRead(ledPin));
  delay(500);
  times ++;
  Serial.println(times);

  if (times > 5) {
    times = 0;
    Serial.println("Go to sleep...");
    energy.PowerDown();
  }
}
