#include <Wire.h>
#include "libs/Adafruit_GFX.h"
#include "StateLoops/avengerCollect.h"
#include "StateLoops/avengerScore.h"
#include "StateLoops/defendGauntlet.h"
#include "StateLoops/lineFollow.h"
#include "StateLoops/stoneCollect.h"
#include "StateLoops/stoneScore.h"
#include "Hardware/HardwareInterface.h"
#include "stateController.h"

#define INTERRUPTPIN PA_8

Adafruit_SSD1306 display(-1);

void interruptRoutine(){
  HardwareInterface::i()->update();
}

void setup() {
    MainState::instance()->setState(lineFollowing);

    //Read push button

    pinMode(INTERRUPTPIN,INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(INTERRUPTPIN),interruptRoutine,RISING);

    //Define main states robot can have
    
    //Declare State Classes
    LineFollow lineFollow;
    AvengerCollect avengerCollect;
    AvengerScore avengerScore;
    StoneCollect stoneCollect;
    StoneScore stoneScore;
    DefendGauntlet defend;

    display.begin(SSD1306_SWITCHCAPVCC,0x3C);
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println("Working");
    display.display();

    int count = 0;
    
    for(;;) {
      display.clearDisplay();
      display.setCursor(0,0);

      //MainState::i()->getState().loop();
      switch(MainState::instance()->getState()){
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
        default:
          break;
      }
      
      count++;
      //int push_Button = digitalRead(PUSHBUTTON);
      //display.println(push_Button == 1 ? "Button Not Pushed":"Button Pushed"); 
      display.println("Main Looping");
      display.println(count);
      //display.print((String)mainState);
      //delay(250);
      display.display();
    }
}

void loop() {}
