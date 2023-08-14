#include <Arduino.h>

//########################################################################### variables
extern int CounterNextPin;         //IRS
extern int CounterConfirmPin;      //
extern int CounterBackPin;         //
extern int CounterUndoPressed;     //
extern bool Lock;              //
extern long volatile ButtonTimer;  //IRS

extern long volatile MainTimer; // millis

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