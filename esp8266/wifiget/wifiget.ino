/*
    This sketch demonstrates how to set up a simple HTTP-like server.
    The server will set a GPIO pin depending on the request
      http://server_ip/gpio/0 will set the GPIO2 low,
      http://server_ip/gpio/1 will set the GPIO2 high
    server_ip is the IP address of the ESP8266 module, will be
    printed to Serial when the module is connected.
*/

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <typeinfo>
// #include <MsTimer2.h>
// #include <CurieTimerOne.h>
// #include <Ticker.h>

#ifndef STASSID
#define STASSID "wifi"
#define STAPSK "passwd"
#endif

// #define LED_BUILTIN D4

const char *ssid = STASSID;
const char *password = STAPSK;

// IPAddress ip(192, 168, 3, 212);
// IPAddress gateway(192, 168, 11, 2);
// IPAddress subnet(255, 255, 255, 0);
// IPAddress dns(2, 6, 4, 66);

String get_url = "/dnsup?domain=iot&ip=";

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);
// Ticker ticker;
WiFiClient client;
// Ticker blinker;

void setup()
{
  Serial.begin(115200);

  // prepare LED
  pinMode(D4, OUTPUT);
  digitalWrite(D4, 1);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print(F("Connecting to "));
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  // WiFi.config(ip, gateway, subnet, dns);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(F("."));
  }
  Serial.println();
  Serial.println(F("WiFi connected"));

  // Start the server
  server.begin();
  Serial.println(F("Server started"));

  // Print the IP address
  Serial.println(WiFi.localIP());

  get_url += WiFi.localIP().toString().c_str();

  // 中断设置函数，每 5s 进入一次中断
  // MsTimer2::set(5000, get_c22);
  //开始计时
  // MsTimer2::start();

  // CurieTimerOne.start(5000, get_c22);
  // blinker.attach(5, get);
}

void loop()
{
  get_c22();
  for (int i = 1; i < 30000000; i++)
  {
    // printf("This loop will run forever.\n");
    web_server();
  }
  // delay(5000);
}

void get_c22()
{
  // String myip;
  // Serial.print(WiFi.localIP().toString().c_str());
  // myip = F(WiFi.localIP());
  get("malu.me", 80, get_url);
}

void get(String host, int httpsPort, String url)
{

  Serial.print("connecting to ");
  Serial.println(host);

  /**
   * 测试是否正常连接
   */
  if (!client.connect(host, httpsPort))
  {
    Serial.println("connection failed");
    return;
  }
  delay(10);

  String postRequest = (String)("GET ") + url + " HTTP/1.1\r\n" +
                       "Content-Type: text/html;charset=utf-8\r\n" +
                       "Host: " + host + "\r\n" +
                       "User-Agent: BFD\r\n" +
                       "Connection: Keep Alive\r\n\r\n";
  Serial.println(postRequest);
  client.print(postRequest); // 发送HTTP请求

  /**
   * 展示返回的所有信息
   */
  String line = client.readStringUntil('\n');
  while (line.length() != 0)
  {
    Serial.print(line);
    line = client.readStringUntil('\n');
  }
  Serial.println(line);
  client.stop();
}

void web_server()
{
  // Check if a client has connected
  WiFiClient client_2 = server.available();
  if (!client_2)
  {
    return;
  }
  // Serial.println(F("new client"));

  client_2.setTimeout(5000); // default is 1000

  // Read the first line of the request
  String req = client_2.readStringUntil('\r');
  Serial.println(F("request: "));
  Serial.println(req);

  // Match the request
  int val;
  if (req.indexOf(F("/gpio/0")) != -1)
  {
    val = 0;
  }
  else if (req.indexOf(F("/gpio/1")) != -1)
  {
    val = 1;
  }
  else
  {
    Serial.println(F("invalid request"));
    val = digitalRead(D4);
  }
  // Serial.println(D0);
  // Serial.println(D1);
  // Serial.println(D2);
  // Serial.println(D3);
  // Serial.println(D4);
  // Serial.println(D5);
  // Serial.println(D6);
  // Serial.println(D7);
  // Serial.println(D8);
  // Serial.println(D9);
  // Serial.println(A0);

  // Set LED according to the request
  digitalWrite(D4, val);

  // read/ignore the rest of the request
  // do not client.flush(): it is for output only, see below
  while (client_2.available())
  {
    // byte by byte is not very efficient
    client_2.read();
  }

  // Send the response to the client
  // it is OK for multiple small client.print/write,
  // because nagle algorithm will group them into one single packet
  client_2.print(F("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n"));
  client_2.print((val) ? F("high") : F("low"));
  client_2.print(F("<br><br>Click <a href='/gpio/1'>here</a> to switch LED GPIO off, or <a href='/gpio/0'>here</a> to switch LED GPIO on.</html>"));

  // The client will actually be *flushed* then disconnected
  // when the function returns and 'client' object is destroyed (out-of-scope)
  // flush = ensure written data are received by the other side
  // Serial.println(F("Disconnecting from client"));
}