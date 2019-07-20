#include <Wire.h>
#include "libs/Adafruit_GFX.h"
#include "StateLoops/avengerCollect.h"
#include "StateLoops/avengerScore.h"
#include "StateLoops/defendGauntlet.h"
#include "StateLoops/lineFollow.h"
#include "StateLoops/stoneCollect.h"
#include "StateLoops/stoneScore.h"
#include "StateLoops/debugging.h"
#include "Hardware/HardwareInterface.h"
#include "stateController.h"
#include "stm32/HardwareTimer.h"
#include <servo.h>

#define INTERRUPTPIN PA_8
#define TOGGLE_SWITCH PB3

Adafruit_SSD1306 display(-1);

void setup() {
  Serial.begin(115200);
  delay(3000);
    
  //Declare State Classes
  LineFollow lineFollow;
  AvengerCollect avengerCollect;
  AvengerScore avengerScore;
  StoneCollect stoneCollect;
  StoneScore stoneScore;
  DefendGauntlet defend;
  Debugging debug;

  display.begin(SSD1306_SWITCHCAPVCC,0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Working");
  Serial.print("Working ");
  display.display();

  MainState::instance()->setState(avengerCollecting);
  // Serial.print(MainState::instance()->getState());
  int count = 0;
    for(;;) {

      if(digitalRead(TOGGLE_SWITCH)){
        MainState::instance()->setState(debugging);
      }
      else if(digitalRead(TOGGLE_SWITCH) == 0 && MainState::instance()->getState()==debugging){
        display.clearDisplay();
        display.setCursor(0,0);
        display.println("Line Following");
        display.display();
        MainState::instance()->setState(lineFollowing);
      }

      //MainState::i()->getState().loop();
      switch(MainState::instance()->getState())
      {
        case(lineFollowing):
          lineFollow.loop();
          break;
        case(avengerCollecting):
          avengerCollect.loop();
          break;
        case(avengerScoring):
          avengerScore.loop();
          break;
        case(stoneCollecting):
          stoneCollect.loop();
          break;
        case(stoneScoring):
          stoneScore.loop();
          break;
        case(defending):
          defend.loop();
          break;
        case(debugging):
          debug.loop();
          break;
        default:
          break;
      }
      HardwareInterface::i()->update();
      count++;
      //int push_Button = digitalRead(PUSHBUTTON);
      //display.println(push_Button == 1 ? "Button Not Pushed":"Button Pushed"); 
      //display.println("Main Looping");
      
      //display.print((String)mainState);
      //delay(250);

    }
}

void loop() {}