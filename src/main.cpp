//########################################################################### Include
#include <Arduino.h>
#include "CriticalFunctions.cpp"
#include "RTClib.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHT.h"
#include "Display.cpp"
#include "ButtonsLogic.cpp"
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
bool ScreanRefreash = 0;

unsigned long MainTimer; // millis
unsigned long  ScreenFrequency;
unsigned long  FunctionalTimer;

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
volatile float SetTemperature = 33;

volatile boolean AllTimeHeating;

volatile long SetStartDayTemperature = 0;
volatile long SetEndDayTemperature = 23;

volatile long SetStartDayLight = 8;
volatile long SetEndDayLight = 22;

boolean Test = false;
boolean HeatingTest = false;
boolean MainLightTest = false;
boolean LedTest = false;
//########################################################################### Objects
ButtonHandlaer ButtonsObj;
DisplayHandlaer DisplayObj;
CriticalFunctions FuncionalObj;
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
  DisplayObj.DefaultScrean();
}
//########################################################################### Loop
void loop() {
  MainTimer = millis();
  if(ScreanRefreash == 1) {
    ScreanRefreash = 0;
  }
  if(MainTimer - ScreenFrequency >= 100) {
    ScreenFrequency = MainTimer;
    ScreanRefreash = 1;
  }
  if(MainTimer - FunctionalTimer >= 500) {
    FunctionalTimer = MainTimer;
    FuncionalObj.LightHandler();
    FuncionalObj.HeatingTimeHandlaer();
  }
}
//########################################################################### ISR
void ConfirmPressed() {
  if(Lock == 0) {
    Lock = 1;
    ButtonTimer = MainTimer;
    ++CounterConfirmPin;
    ButtonsObj.Confirm();
    ScreanRefreash = 1;
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
    ButtonsObj.Back();
    ScreanRefreash = 1;
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
    ButtonsObj.Next();
    ScreanRefreash = 1;
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
    ButtonsObj.Undo();
    ScreanRefreash = 1;
  }
  if(ButtonTimer - MainTimer >=50) {
    Lock = 0;
  }
}
  //########################################################################### Background





