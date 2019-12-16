/**
 * rc522接线
 * SDA->D8
 * SCK->D5
 * MOSI->D7
 * MISO->D5
 * 3.3V->D3
 * 
 * ESP8266接线
 * RST->D0
 * 
 * 舵机接线
 * PIN->D1
 */
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#define RST_PIN 10 // 配置针脚
#define SS_PIN 15
MFRC522 mfrc522(SS_PIN, RST_PIN); // 创建新的RFID实例
MFRC522::MIFARE_Key key;
unsigned long starttime;
Servo servo;

//打开舵机
void open()
{
    servo.attach(D1);
    //you begin your own personal code for servo here
    Serial.println("open");
    servo.write(180);
    delay(1000);
    servo.write(0);
    delay(1000);
}

// 匹配用户
void CheckUser(unsigned char *id)
{
    if (id[0] == 0x00 && id[1] == 0x00 && id[2] == 0x00 && id[3] == 0x00)
    {
        Serial.println("Hello Malu!");
        digitalWrite(LED_BUILTIN, LOW); //把板载led点亮
        open();
    }
}

void go_poweroff()
{
    Serial.println("go to sleep!!!");
    ESP.deepSleep(3e6); // 10 seconds
}

void setup()
{
    //给D3引脚高电平，给rf522供电
    pinMode(D3, OUTPUT);
    digitalWrite(D3, HIGH);

    //先把板载led关闭
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);

    Serial.begin(115200); // 设置串口波特率为115200
    SPI.begin();          // SPI开始
    mfrc522.PCD_Init();   // Init MFRC522 card
    Serial.println();
    Serial.println("Init MFRC522 card");
    starttime = millis(); //初始化时间
}

void loop()
{
    // digitalWrite(D1, HIGH);
    // 寻找新卡
    if (!mfrc522.PICC_IsNewCardPresent())
    {
        // Serial.println("没有找到卡");
        // if ((millis() - starttime) > 1e3)
        // {
        go_poweroff();
        // }
        return;
    }

    // 选择一张卡
    if (!mfrc522.PICC_ReadCardSerial())
    {
        Serial.println("没有卡可选");
        return;
    }

    // 显示卡片的详细信息
    // Serial.print(F("卡片 UID:"));
    Serial.print("卡片 UID:");
    dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
    Serial.print(" ");
    printDec(mfrc522.uid.uidByte, mfrc522.uid.size);
    Serial.println();
    // Serial.print(F("卡片类型: "));
    Serial.print("卡片类型: ");
    MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
    Serial.println(mfrc522.PICC_GetTypeName(piccType));

    //校验用户
    CheckUser(mfrc522.uid.uidByte);

    // // 检查兼容性
    // if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI && piccType != MFRC522::PICC_TYPE_MIFARE_1K && piccType != MFRC522::PICC_TYPE_MIFARE_4K)
    // {
    //     // Serial.println(F("仅仅适合Mifare Classic卡的读写"));
    //     Serial.println("仅仅适合Mifare Classic卡的读写");
    //     return;
    // }

    // MFRC522::StatusCode status;
    // if (status != MFRC522::STATUS_OK)
    // {
    //     // Serial.print(F("身份验证失败？或者是卡链接失败"));
    //     Serial.print("身份验证失败？或者是卡链接失败");
    //     Serial.println(mfrc522.GetStatusCodeName(status));
    //     return;
    // }
    //停止 PICC
    mfrc522.PICC_HaltA();
    //停止加密PCD
    mfrc522.PCD_StopCrypto1();

    return;
}

/**
   将字节数组转储为串行的十六进制值
*/
void dump_byte_array(byte *buffer, byte bufferSize)
{
    for (byte i = 0; i < bufferSize; i++)
    {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], HEX);
    }
}

void printDec(byte *buffer, byte bufferSize)
{
    for (byte i = 0; i < bufferSize; i++)
    {
        // Serial.print(buffer[i] < 0x10 ? " 0" : "");
        Serial.print(buffer[i], DEC);
    }
}