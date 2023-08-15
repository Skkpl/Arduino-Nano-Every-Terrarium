#include <Arduino.h>

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