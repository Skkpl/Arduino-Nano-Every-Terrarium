//########################################################################### Include
#include <Arduino.h>
#include "Bitmaps.cpp"
#include "CriticalFunctions.cpp"
#include "RTClib.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHT.h"
//########################################################################### Define
#define Next_pin 9
#define confirm_pin 8
#define back_pin 12
#define Undo_Pressed 10
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 6
#define OLED_RESET -1
//########################################################################### Library components
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
DHT dhtP(5, DHT11);
DHT dhtL(3, DHT11);
DHT dhtg(4,DHT11);
RTC_DS1307 rtc;
//########################################################################### variables
int CounterNextPin;         //IRS
int CounterConfirmPin;      //
int CounterBackPin;         //
int CounterUndoPressed;     //
bool Lock = 0;              //
long volatile ButtonTimer;  //IRS

long volatile MainTimer; // millis

int DisplayLevel = 0;
int DisplayLevelLocker = 0;

int DsHour;
int DsMinute;

float Temperature;
float Temperature2;
float Temperature3;
float Humidity;
float Humidity2;
float Humidity3;

bool heating = 0;
bool Light = 0;

int LightLevelFirstHalf = 0;
int LightLevelSecenfHalf = 0;
//########################################################################### User Set Variables
volatile float SetTemperature;

volatile boolean AllTimeHeating;

volatile long SetStartDayTemperature;
volatile long SetEndDayTemperature;

volatile long SetStartDayLight;
volatile long SetEndDayLight;

boolean Test = false;
boolean HeatingTest = false;
boolean MainLightTest = false;
boolean LedTest = false;
//########################################################################### Structures

//########################################################################### Functions declaration
void ImageDisplay();
void Backgroudn();
//########################################################################### Function declarations Interrupt
void ConfirmPressed();
void NextPressed();
void BackPressed();
void UndoPressed();
//########################################################################### Setup
void setup() {
  Serial.begin(115200);

  pinMode(7, OUTPUT); // Bulp
  pinMode(5, OUTPUT); // Heatint Mat
  pinMode(11, OUTPUT); // White Led Tranzistor
  pinMode(5, OUTPUT); // secend tranzistor doesnt work

  pinMode(confirm_pin, INPUT_PULLUP);
  pinMode(back_pin, INPUT_PULLUP);
  pinMode(Next_pin, INPUT_PULLUP);
  pinMode(Undo_Pressed, INPUT_PULLUP);

  attachInterrupt(confirm_pin, ConfirmPressed, CHANGE);
  attachInterrupt(back_pin, BackPressed, CHANGE);
  attachInterrupt(Next_pin, NextPressed, CHANGE);
  attachInterrupt(Undo_Pressed, UndoPressed, CHANGE);

  display.begin(SSD1306_SWITCHCAPVCC, (0x3c)); //i2c display adress
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  dhtL.begin();
  dhtP.begin();
  dhtg.begin();
  display.clearDisplay();
  display.drawBitmap(0, 0, Lizard, 128, 64, WHITE);
  display.display();

  #ifndef ESP8266
  while (!Serial);
  #endif
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }
    if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  delay(3000);
}
//########################################################################### Loop
void loop() {
  // put your main code here, to run repeatedly:
}
//########################################################################### ISR
void ConfirmPressed() {
  if(Lock == 0) {
    Lock = 1;
    ButtonTimer = MainTimer;
    ++CounterConfirmPin;
  }
  if(ButtonTimer - MainTimer >=50) {
    Lock = 0;
  }

}
void BackPressed() {
  if(Lock == 0) {
    Lock = 1;
    ButtonTimer = MainTimer;
    ++CounterBackPin;
  }
  if(ButtonTimer - MainTimer >=50) {
    Lock = 0;
  }
}

void NextPressed() {
  if(Lock == 0) {
    Lock = 1;
    ButtonTimer = MainTimer;
    ++CounterNextPin;
  }
  if(ButtonTimer - MainTimer >=50) {
    Lock = 0;
  }
}

void UndoPressed() {
  if(Lock == 0) {
    Lock = 1;
    ButtonTimer = MainTimer;
    ++CounterUndoPressed;
  }
  if(ButtonTimer - MainTimer >=50) {
    Lock = 0;
  }
}
//########################################################################### Critical Functions
class CriticalFunctions{
  public:

    void LightHandler() {
      if(SetStartDayLight >= DsHour && SetEndDayLight < DsHour) {
        long HoursToGo;
        HoursToGo = SetEndDayLight - SetStartDayLight;
        LightLevelFirstHalf = map(LightLevelFirstHalf, SetStartDayLight, (SetStartDayLight + (HoursToGo / 2)), 20, 255);
        LightLevelSecenfHalf = map(LightLevelFirstHalf, ((HoursToGo / 2) + SetStartDayLight), SetEndDayLight, 255, 20);
        if(DsHour >= (SetStartDayLight && SetStartDayLight + (HoursToGo / 2)) < DsHour) {
          analogWrite(11, LightLevelFirstHalf);
        }else if(DsHour >= (SetStartDayLight + (HoursToGo / 2)) && SetEndDayLight < DsHour){
          analogWrite(11, LightLevelSecenfHalf);
        } else {
          digitalWrite(11, LOW);
        }
      } else {
        digitalWrite(11, LOW);
      }
    }

    void HeatingTimeHandlaer() {
      if(SetStartDayTemperature >= DsHour && SetEndDayTemperature < DsHour) {
        if((SetTemperature  - 2 ) < Temperature) {
          heating = 1;
        }else if((SetTemperature + 2 ) > Temperature) {
         heating = 0;
        }
      }else {
         heating = 0;
      }
    }
};
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
//########################################################################### Button Handlaer
class ButtonHandlaer{
  private:
    void Next() {
      switch (DisplayLevelLocker) {
        case 0:
          switch (DisplayLevel) {
            case 0:
              DisplayLevel++;
              break;
            
            case 1:
              DisplayLevel++;
              break;

            case 2:
              DisplayLevel++;
              break;

            case 3:
              DisplayLevel = 0;
              break;
            
            case 4:
              DisplayLevel++;
              break;

            case 5:
              DisplayLevel = 4;
              break;

            case 6:
              DisplayLevel++;
              break;

            case 7:
              DisplayLevel++;
              break;

            case 8:
              DisplayLevel++;
              break;

            case 9:
              DisplayLevel = 6;
              break;

            case 10:
              break;

            case 11:
              break;

            case 12:
              DisplayLevel++;
              break;
            
            case 13:
              DisplayLevel++;
              break;

            case 14:
              DisplayLevel++;
              break;

            case 15:
              DisplayLevel = 12;
              break;
          }
          break;

        case 1:  // Time Config light Start time hour
          SetStartDayLight++;
          break;

        case 2: //Time config light End Time Hour
          SetEndDayLight++;
          break;

        case 3: // Temperature Config Start hour
          SetStartDayTemperature++;
          break;

        case 4: // Temperature Config Stop hour
          SetEndDayTemperature++;
          break;

        case 5: // Temperature Config All time
         AllTimeHeating = !AllTimeHeating;
          break;

        case 6:// Temperature Config Temperature Set
          SetTemperature++;
          break;

        case 7: // Self Test off
          DisplayLevel++;
          break;

        case 8: // Self Test On
          DisplayLevel--;
          break;

        case 9: // Self Test Heating
          HeatingTest = !HeatingTest;
          break;

        case 10: // Self Test Light
          MainLightTest = !MainLightTest;
          break;

        case 11: // Self Test Led
          LedTest = !LedTest;
          break;
        }
    }

    void Confirm() {
      switch (DisplayLevel) {
        case 0:
          
          break;
      }
    }

    void Back() {
      if (DisplayLevel <= 4) {
        DisplayLevel = 0;
      }
    }


  void Undo() {
    switch (DisplayLevelLocker) {
        case 0:
          switch (DisplayLevel) {
            case 0:
              DisplayLevel = 3;
              break;
            
            case 1:
              DisplayLevel--;
              break;

            case 2:
              DisplayLevel--;
              break;

            case 3:
              DisplayLevel--;
              break;
            
            case 4:
              DisplayLevel = 5;
              break;

            case 5:
              DisplayLevel--;
              break;

            case 6:
              DisplayLevel = 9;
              break;

            case 7:
              DisplayLevel--;
              break;

            case 8:
              DisplayLevel--;
              break;

            case 9:
              DisplayLevel--;
              break;

            case 10:
              break;

            case 11:
              break;

            case 12:
              DisplayLevel = 15;
              break;
            
            case 13:
              DisplayLevel--;
              break;

            case 14:
              DisplayLevel--;
              break;

            case 15:
              DisplayLevel--;
              break;
          }
          break;

        case 1:  // Time Config light Start time hour
          SetStartDayLight--;
          break;

        case 2: //Time config light End Time Hour
          SetEndDayLight--;
          break;

        case 3: // Temperature Config Start hour
          SetStartDayTemperature--;
          break;

        case 4: // Temperature Config Stop hour
          SetEndDayTemperature--;
          break;

        case 5: // Temperature Config All time
         AllTimeHeating = !AllTimeHeating;
          break;

        case 6:// Temperature Config Temperature Set
          SetTemperature--;
          break;

        case 7: // Self Test off
          DisplayLevel--;
          break;

        case 8: // Self Test On
          DisplayLevel++;
          break;

        case 9: // Self Test Heating
          HeatingTest = !HeatingTest;
          break;

        case 10: // Self Test Light
          MainLightTest = !MainLightTest;
          break;

        case 11: // Self Test Led
          LedTest = !LedTest;
          break;
        }
  }


};
  //########################################################################### Background





