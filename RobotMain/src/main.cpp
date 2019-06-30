#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <FreeMono9pt7b.h>
#include "StateLoops/avengerCollect.h"
#include "StateLoops/avengerScore.h"
#include "StateLoops/defendGauntlet.h"
#include "StateLoops/lineFollow.h"
#include "StateLoops/stoneCollect.h"
#include "StateLoops/stoneScore.h"
#include "config.h"

#define LINE_SENSOR_1 PA4
#define LINE_SENSOR_2 PA5
#define LINE_SENSOR_3 PA6
#define LINE_SENSOR_4 PA7

Adafruit_SSD1306 display(-1);

void setup() {
    MainState::i()->setState(lineFollowing);

    //Read push button
    pinMode(LINE_SENSOR_1,INPUT);
    pinMode(LINE_SENSOR_2,INPUT);
    pinMode(LINE_SENSOR_3,INPUT);
    pinMode(LINE_SENSOR_4,INPUT);

    //Define main states robot can have
    
    //Declare State Classes
    StateLoops::LineFollow lineFollow;
    StateLoops::AvengerCollect avengerCollect;
    StateLoops::AvengerScore avengerScore;
    StateLoops::StoneCollect stoneCollect;
    StateLoops::StoneScore stoneScore;
    StateLoops::DefendGauntlet defend;

    //Hardware::QRD qrd1;
    //Hardware::Motor leftMotor;

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
      switch(MainState::i()->getState()){
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
      display.print(analogRead(LINE_SENSOR_1), DEC);
      display.print(" ");
      display.print(analogRead(LINE_SENSOR_2), DEC);
      //display.print(" ");
      //display.print(analogRead(LINE_SENSOR_3), DEC);
      //display.print(" ");
      //display.print(analogRead(LINE_SENSOR_4), DEC);
      //display.print((String)mainState);
      //delay(250);
      display.display();
    }
}

void loop() {}
