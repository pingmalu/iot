void setup()
{
    Serial.begin(115200);
    //Serial.setTimeout(2000);

    // Wait for serial to initialize.
    while (!Serial)
    {
    }

    Serial.println("I'm awake.");

    Serial.println("Going into deep sleep for 10 seconds");

    // 一但遇到以下方法ESP就会进入deepSleep状态
    ESP.deepSleep(10e6); // 20e6 is 20 microseconds
}

void loop()
{
}