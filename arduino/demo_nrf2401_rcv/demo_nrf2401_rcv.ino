/**
 * 接收机
 *
 * Pins:
 * Hardware SPI:
 * MISO -> 12
 * MOSI -> 11
 * SCK -> 13
 *
 * Configurable:
 * CE -> 8
 * CSN -> 7
 *
 * Note: To see best case latency comment out all Serial.println
 * statements not displaying the result and load 
 * 'ping_server_interupt' on the server.
 */

#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>

byte joyStick[2];

void setup() {
  
  Serial.begin(9600);
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();     
  Mirf.setRADDR((byte *)"serv1");   
  Mirf.payload = sizeof(joyStick);
  
  /*
   * Write channel and payload config then power up reciver.
   */
   
  /*
   * To change channel:
   * 
   * Mirf.channel = 10;
   *
   * NB: Make sure channel is legal in your area.
   */
   
  Mirf.config();

  Serial.println("Receiver ready");
  
}

void loop() {
  
  while(!Mirf.dataReady()){
  }

  Mirf.getData((byte *) &joyStick);
  
  int vx=joyStick[0];
  int vy=joyStick[1];

  if (vx < 130) {
    
  }   // step forward
  
  if (vx > 130) {
    
  }  // step backward
 
  if (vy < 123) {  

  } // step left
  
  if (vy > 123) {  
  } // step right
  
  Serial.print("X:");
  Serial.print(vx);
  Serial.print(" Y:");
  Serial.println(vy);  

}