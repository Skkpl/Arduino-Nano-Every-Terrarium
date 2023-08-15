#include <Arduino.h>
#include "Bitmaps.cpp"
#include "RTClib.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHT.h"

extern Adafruit_SSD1306 display;
//########################################################################### variables
extern int CounterNextPin;         //IRS
extern int CounterConfirmPin;      //
extern int CounterBackPin;         //
extern int CounterUndoPressed;     //
extern bool Lock;              //
extern long volatile ButtonTimer;  //IRS

extern unsigned long MainTimer; // millis

extern int DisplayLevel;
extern int DisplayLevelLocker;

extern int DsHour;
extern int DsMinute;

extern float Temperature;
extern float Temperature2;
extern float Temperature3;
extern float Humidity;
extern float Humidity2;
extern float Humidity3;

extern bool heating;
extern bool Light;

extern int LightLevelFirstHalf;
extern int LightLevelSecenfHalf;
//########################################################################### User Set Variables
extern volatile float SetTemperature;

extern volatile boolean AllTimeHeating;

extern volatile long SetStartDayTemperature;
extern volatile long SetEndDayTemperature;

extern volatile long SetStartDayLight;
extern volatile long SetEndDayLight;

extern boolean Test;
extern boolean HeatingTest;
extern boolean MainLightTest;
extern boolean LedTest;
//########################################################################### Display
class DisplayHandlaer{
  public:
    void DefaultScrean() {
      display.clearDisplay();
      display.drawBitmap(0, 0, Thermometer, 24, 42, WHITE);
      display.drawBitmap(65, 13, Higrometer, 19, 25, WHITE);
      display.setCursor(25, 20);
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.print(Temperature);
      display.setCursor(90, 20);
      display.print(Humidity, "%");
      display.setCursor(32, 46);
      display.print(DsHour, ":");
      display.print(DsMinute);
      display.display();
    }
    void ImageDisplay() {
      display.clearDisplay();
      switch (DisplayLevel) {
        case 0:
          display.setTextSize(1);
          display.setCursor(0,0);
          display.print("> Time Config Light");
          display.setCursor(0,12);
          display.print("  Temperature Config");
          display.setCursor(0,24);
          display.print("  Meters Reading");
          display.setCursor(0,36);
          display.print("  Output Test");
          display.setCursor(30, 55);
          display.print("Version 2.0");
          break;

        case 1:
          display.setTextSize(1);
          display.setCursor(0,0);
          display.print("  Time Config Light");
          display.setCursor(0,12);
          display.print("> Temperature Config");
          display.setCursor(0,24);
          display.print("  Meters Reading");
          display.setCursor(0,36);
          display.print("  Output Test");
          display.setCursor(30, 55);
          display.print("Version 2.0");
          break;

        case 2:
          display.setTextSize(1);
          display.setCursor(0,0);
          display.print("  Time Config Light");
          display.setCursor(0,12);
          display.print("  Temperature Config");
          display.setCursor(0,24);
          display.print("> Meters Reading");
          display.setCursor(0,36);
          display.print("  Output Test");
          display.setCursor(30, 55);
          display.print("Version 2.0");
          break;

        case 3:
          display.setTextSize(1);
          display.setCursor(0,0);
          display.print("  Time Config Light");
          display.setCursor(0,12);
          display.print("  Temperature Config");
          display.setCursor(0,24);
          display.print("  Meters Reading");
          display.setCursor(0,36);
          display.print("> Output Test");
          display.setCursor(30, 55);
          display.print("Version 2.0");
          break;

        case 4: // Inside Time Config Light
          display.setTextSize(1);
          display.setCursor(0,0);
          display.print("> Start hour ");
          display.print(SetStartDayLight);
          display.setCursor(0,10);
          display.print("  Stop hour");
          display.print(SetEndDayLight);
          break;

        case 5:// Inside Time Config Light
          display.setTextSize(1);
          display.setCursor(0,0);
          display.print("  Start hour ");
          display.print(SetStartDayLight);
          display.setCursor(0,10);
          display.print("> Stop hour ");
          display.print(SetEndDayLight);
          break;

        case 6: // Inside Config Temperature
          display.setTextSize(1);
          display.setCursor(0,0);
          display.print("> Start hour ");
          display.print(SetStartDayTemperature);
          display.setCursor(0,10);
          display.print("  Stop hour ");
          display.print(SetEndDayTemperature);
          display.setCursor(0, 20);
          display.print("  All Time On = ");
          display.print(AllTimeHeating);
          display.setCursor(0,30);
          display.print("  Set Temp ");
          display.print(SetTemperature);
          break;

        case 7: // Inside Config Temperature
          display.setTextSize(1);
          display.setCursor(0,0);
          display.print("  Start hour ");
          display.print(SetStartDayTemperature);
          display.setCursor(0,10);
          display.print("> Stop hour ");
          display.print(SetEndDayTemperature);
          display.setCursor(0, 20);
          display.print("  All Time On = ");
          display.print(AllTimeHeating);
          display.setCursor(0,30);
          display.print("  Set Temp ");
          display.print(SetTemperature);
          break;

          case 8: // Inside Config Temperature
          display.setTextSize(1);
          display.setCursor(0,0);
          display.print("  Start hour ");
          display.print(SetStartDayTemperature);
          display.setCursor(0,10);
          display.print("  Stop hour ");
          display.print(SetEndDayTemperature);
          display.setCursor(0, 20);
          display.print("> All Time On = ");
          display.print(AllTimeHeating);
          display.setCursor(0,30);
          display.print("  Set Temp ");
          display.print(SetTemperature);
          break;

        case 9: // Inside Config Temperature
          display.setTextSize(1);
          display.setCursor(0,0);
          display.print("  Start hour ");
          display.print(SetStartDayTemperature);
          display.setCursor(0,10);
          display.print("  Stop hour ");
          display.print(SetEndDayTemperature);
          display.setCursor(0, 20);
          display.print("  All Time On = ");
          display.print(AllTimeHeating);
          display.setCursor(0,30);
          display.print("> Set Temp ");
          display.print(SetTemperature);
          break;

        case 10: // Meters Reading
          display.setTextSize(1);
          display.setCursor(0,0);
          display.print("Down Left ");
          display.print(Temperature);
          display.print("C ");
          display.print(Humidity);
          display.print("%");
          display.setCursor(0,12);
          display.print("Down Right ");
          display.print(Temperature2);
          display.print("C ");
          display.print(Humidity2);
          display.print("%");
          break;

        case 11: // Tests OFF
          display.setTextSize(1);
          display.setCursor(0,0);
          display.print("> Self Test = ");
          display.print("OFF");
          break;

        case 12: // Tests ON
          display.setTextSize(1);
          display.setCursor(0,0);
          display.print("> Self Test = ");
          display.print("ON");
          display.setCursor(0,12);
          display.print("  Heater = ");
          display.print(HeatingTest);
          display.setCursor(0,24);
          display.print("  Main Light = ");
          display.print(MainLightTest);
          display.setCursor(0,36);
          display.print("  LED Test = ");
          display.print(LedTest);
          break;

        case 13: // Tests ON
          display.setTextSize(1);
          display.setCursor(0,0);
          display.print("  Self Test = ");
          display.print("ON");
          display.setCursor(0,12);
          display.print("> Heater = ");
          display.print(HeatingTest);
          display.setCursor(0,24);
          display.print("  Main Light = ");
          display.print(MainLightTest);
          display.setCursor(0,36);
          display.print("  LED Test = ");
          display.print(LedTest);
          break;

        case 14: // Tests ON
          display.setTextSize(1);
          display.setCursor(0,0);
          display.print("  Self Test = ");
          display.print("ON");
          display.setCursor(0,12);
          display.print("  Heater = ");
          display.print(HeatingTest);
          display.setCursor(0,24);
          display.print("> Main Light = ");
          display.print(MainLightTest);
          display.setCursor(0,36);
          display.print("  LED Test = ");
          display.print(LedTest);
          break;

        case 15: // Tests ON
          display.setTextSize(1);
          display.setCursor(0,0);
          display.print("  Self Test = ");
          display.print("ON");
          display.setCursor(0,12);
          display.print("  Heater = ");
          display.print(HeatingTest);
          display.setCursor(0,24);
          display.print("  Main Light = ");
          display.print(MainLightTest);
          display.setCursor(0,36);
          display.print("> LED Test = ");
          display.print(LedTest);
          break;
      }
    }
};