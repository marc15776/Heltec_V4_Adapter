#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "esp_sleep.h"

#define VEXT        36
#define OLED_RST    21
#define OLED_SDA    17
#define OLED_SCL    18

#define BUTTON_PIN  0

Adafruit_SSD1306 display(128, 64, &Wire, OLED_RST);

float homeBattery = 0.0;

// Platzhalter bis LoRa kommt
float fenceBattery = 4.02;
float fenceVoltage = 7420;

void readHomeBattery()
{
    pinMode(37, OUTPUT);
    digitalWrite(37, HIGH);

    delay(5);

    int bat_mv = analogReadMilliVolts(1);

    homeBattery = (bat_mv * 4.9f) / 1000.0f;
}

void drawScreen()
{
    display.clearDisplay();

    display.setTextColor(SSD1306_WHITE);

    display.setTextSize(1);

    display.setCursor(0, 0);
    display.print("HOME ");
    display.print(homeBattery, 2);
    display.println("V");

    display.setCursor(0, 12);
    display.print("ZAUN ");
    display.print(fenceBattery, 2);
    display.println("V");

    display.drawLine(0, 25, 127, 25, SSD1306_WHITE);

    display.setTextSize(2);

    display.setCursor(0, 38);
    display.print((int)fenceVoltage);
    display.print("V");

    display.display();
}

void powerOffOLED()
{
    display.clearDisplay();
    display.display();

    digitalWrite(VEXT, HIGH);
}

void goToSleep()
{
    Serial.println("Deep Sleep");

    powerOffOLED();

    delay(100);

    esp_sleep_enable_ext0_wakeup(
        GPIO_NUM_0,
        0
    );

    esp_deep_sleep_start();
}

void setup()
{
    Serial.begin(115200);

    delay(500);

    esp_sleep_wakeup_cause_t wakeup_reason =
        esp_sleep_get_wakeup_cause();

    if(wakeup_reason == ESP_SLEEP_WAKEUP_EXT0)
    {
        Serial.println("Wakeup durch Button");
    }
    else
    {
        Serial.println("Erster Start");
    }

    // OLED Strom EIN
    pinMode(VEXT, OUTPUT);
    digitalWrite(VEXT, LOW);

    delay(100);

    Wire.begin(OLED_SDA, OLED_SCL);

    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
        Serial.println("OLED Fehler");

        while(true)
        {
            delay(1000);
        }
    }

    analogReadResolution(12);

    readHomeBattery();

    drawScreen();

    Serial.print("Home Battery: ");
    Serial.println(homeBattery, 2);

    Serial.println("Anzeige 15 Sekunden");

    delay(15000);

    goToSleep();
}

void loop()
{
}