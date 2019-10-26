#include <SPI.h>
#include <RFID.h> 

RFID rfid(10,5);   //D10--读卡器MOSI引脚、D5--读卡器RST引脚

int led = 9;
int relay=7;

bool state=true;
void setup()
{
  Serial.begin(9600);
  SPI.begin();
  rfid.init();
  pinMode(led, OUTPUT);  
  pinMode(relay,OUTPUT);
  digitalWrite(relay,HIGH);
}

void loop()
{
  unsigned char type[MAX_LEN];
  //找卡
  if (rfid.isCard()) {
    Serial.println("Find the card!"); 
    // Show card type
    ShowCardType(type);
    //读取卡序列号
    if (rfid.readCardSerial()) {
      Serial.print("The card's number is  : ");
      Serial.print(rfid.serNum[0],HEX);
      Serial.print(rfid.serNum[1],HEX);
      Serial.print(rfid.serNum[2],HEX);
      Serial.print(rfid.serNum[3],HEX);
      Serial.print(rfid.serNum[4],HEX);
      Serial.println(" ");
      ShowUser(rfid.serNum);
    }
    //选卡，可返回卡容量（锁定卡片，防止多数读取），去掉本行将连续读卡
    Serial.println(rfid.selectTag(rfid.serNum));
  }

  rfid.halt();
}

void ShowCardType( unsigned char* type)
{
  Serial.print("Card type: ");
  if(type[0]==0x04&&type[1]==0x00) 
    Serial.println("MFOne-S50");
  else if(type[0]==0x02&&type[1]==0x00)
    Serial.println("MFOne-S70");
  else if(type[0]==0x44&&type[1]==0x00)
    Serial.println("MF-UltraLight");
  else if(type[0]==0x08&&type[1]==0x00)
    Serial.println("MF-Pro");
  else if(type[0]==0x44&&type[1]==0x03)
    Serial.println("MF Desire");
  else
    Serial.println("Unknown");
}

void ShowUser( unsigned char* id)
{
  //EE 9B 9C 38 D1 
  if( id[0]==0xEE && id[1]==0x9B && id[2]==0x9C && id[3]==0x38 ) {
    Serial.println("Hello Mary!");
    state=RelayStatus(state);
  } 
  else if(id[0]==0x24 && id[1]==0x12 && id[2]==0xE0 && id[3]==0x13) {
    Serial.println("Hello MicroHao!");
    state=RelayStatus(state);
  }
  else{
    Serial.println("Hello unkown guy!");
    BlinkLED();

  }
}
bool RelayStatus(bool status)
{
  if(status)
  {
    digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)

    digitalWrite(relay,LOW);
    return false;
  } 
  digitalWrite(led, LOW);   // turn the LED on (HIGH is the voltage level)

  digitalWrite(relay,HIGH);
  return true;
}
void BlinkLED()
{ 
  digitalWrite(relay,HIGH);
  for(int i=0;i<3;i++)
  {
    digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000); 
    digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);
  } 
}