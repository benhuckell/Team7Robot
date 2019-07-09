#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <FreeMono9pt7b.h>
#include "StateLoops/avengerCollect.h"
#include "StateLoops/avengerScore.h"
#include "StateLoops/defendGauntlet.h"
#include "StateLoops/lineFollow.h"
#include "StateLoops/stoneCollect.h"
#include "StateLoops/stoneScore.h"
#include "stateController.h"
#include "Arduino.h"

#define CONTROL_POT_1 PA_4
#define CONTROL_POT_2 PA_5
#define PUSH_BUTTON_1 PB_4
#define PUSH_BUTTON_2 PB_5
#define TOGGLE_SWITCH PB_3

#define SELECT_PIN_1 PB12
#define SELECT_PIN_2 PB13
#define SELECT_PIN_3 PB14
#define MULTIPLEX_ANALOG_IN PA_6

Adafruit_SSD1306 display(-1);

void setup() {
    int count = 0;

    MainState::i()->setState(lineFollowing);

    //Set up inputs
    pinMode(CONTROL_POT_1,INPUT);
    pinMode(CONTROL_POT_2,INPUT);
    pinMode(PUSH_BUTTON_1,INPUT_PULLDOWN);
    pinMode(PUSH_BUTTON_2,INPUT_PULLDOWN);
    pinMode(TOGGLE_SWITCH,INPUT_PULLDOWN);

    pinMode(SELECT_PIN_1,OUTPUT);
    pinMode(SELECT_PIN_2,OUTPUT);
    pinMode(SELECT_PIN_3,OUTPUT);

    pinMode(MULTIPLEX_ANALOG_IN,INPUT);

    //Declare State Classes
    StateLoops::LineFollow lineFollow;
    StateLoops::AvengerCollect avengerCollect;
    StateLoops::AvengerScore avengerScore;
    StateLoops::StoneCollect stoneCollect;
    StateLoops::StoneScore stoneScore;
    StateLoops::DefendGauntlet defend;

    display.begin(SSD1306_SWITCHCAPVCC,0x3C);
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println("Working");
    display.display();

    for(;;) {
      display.clearDisplay();
      display.setCursor(0,0);

      switch(MainState::i()->getState()){
        case(lineFollowing):
          lineFollow.loop();
          if(digitalRead(PUSH_BUTTON_1)){
            delay(100);
            MainState::i()->setState(avengerCollecting);
          }
          break;
        case(avengerCollecting):
          avengerCollect.loop();
          if(digitalRead(PUSH_BUTTON_1)){
            delay(100);
            MainState::i()->setState(avengerScoring);
          }
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

      display.println(count);
      // //display.print(analogRead(CONTROL_POT_1), DEC);
      // //display.print(" ");
      // display.print(analogRead(CONTROL_POT_2), DEC);
      // display.print(" ");
      // display.print(digitalRead(PUSH_BUTTON_1), DEC);
      // display.print(" ");
      // display.print(digitalRead(PUSH_BUTTON_2), DEC);
      // display.print(" ");
      // display.println(digitalRead(TOGGLE_SWITCH), DEC);

      // Read from QRD sensors
      int QRD_Out[4];
      bool states[4][3] = {{LOW,LOW,LOW},{LOW,LOW,HIGH},{LOW,HIGH,LOW},{LOW,HIGH,HIGH}};

      for(int i=0;i<4;i++){
        digitalWrite(SELECT_PIN_1,states[i][0]);
        digitalWrite(SELECT_PIN_2,states[i][1]);
        digitalWrite(SELECT_PIN_3,states[i][2]);
        QRD_Out[i] = analogRead(MULTIPLEX_ANALOG_IN); //read from first multiplexer
      }

      display.print(QRD_Out[0]);

      display.print(" ");
      display.print(QRD_Out[1]);
      display.print(" ");
      display.print(QRD_Out[2]);
      display.print(" ");
      display.print(QRD_Out[3]);

      display.display();
    }
}

void loop() {}

