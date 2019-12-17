//programm_specific
#include <string>         //to use string
int pin_out_LED;          //LED
int pin_in_BUTTON;        //Button
int pressing;              //remembers if button is pressed
int counter;               //message_out counter
String msg;                //message_out content
String line;               //message_in  content

//time_calculation
unsigned long time_a;      //start_time
unsigned long time_b;      //end_time
bool time_check;           //are we interested in time?
bool received;

//Configure WIFI:
#include <ESP8266WiFi.h>  //Wifi-li brary


//WLAN-Config
const char* ssid     = "YOUR_WIFI_NAME_?";          //Your WIFI Name?
const char* password = "YOUR_WIFI_PASSWORD?";      //Your WIFI Password?

//Host&Client
WiFiClient client;                              //Feather as a client
const char* host = "192.168....IP_AD..";        //My Server(processing) IP Address(Terminal:"ifconfig -a")
const int httpPort = 12345;                     //Servers port


void setup() {

  Serial.begin(115200);                              //baud rate
  pin_out_LED = 14;
  pin_in_BUTTON = 12;
  pressing = 0;
  counter = 0;
  time_a = millis();
  time_b = millis();
  time_check = false;
  received = false;
  pinMode(pin_out_LED, OUTPUT);
  pinMode(pin_in_BUTTON, INPUT);

  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);                         //Connect to WIFI
  digitalWrite(pin_out_LED, HIGH);
  while (WiFi.status() != WL_CONNECTED) {

    delay(500);
    Serial.print(".");
  }
  digitalWrite(pin_out_LED, LOW);                     //We are connected to SSID
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {

  if (!client.connected())
  {
    if (!client.connect(host, httpPort))
    {
      Serial.println("connection failed");
      delay(500);
    }
  } else {
    //Read Sensor
    if (digitalRead(pin_in_BUTTON))   //if( 1==digitalRead(..)) Button pressed?
    {
      if (pressing == 0)
      {
        counter++;
        if (counter > 100)
        {
          counter = 0;
        }

        msg = String(counter) + ": s \n\r";
        client.print(msg);                      //SEND to Server

        pressing = 1;
        time_check = true;
        received = false;
      }
    } else {
      pressing = 0;
    }

    while (client.available())
    {
      //RADD SERVVVVVVVVVVVVVER
      line = client.readStringUntil('\r');    //READ from Server
      if (line.endsWith("1"))
      {
        digitalWrite(pin_out_LED, HIGH);
        received = true;
      } else if (line.endsWith("0"))
      {
        digitalWrite(pin_out_LED, LOW);
        received = true;
      }
    }

    //To calculate Send_Time
    if (time_check)
    {
      if (received)
      {
        time_b = millis();
        msg = String(time_b - time_a) + "transfer_time";
        Serial.println(msg);
        time_check = false;
        received = false;
      }
    } else
    {
      time_a = millis();
    }
  }

}
