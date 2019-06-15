/*
 * Test OLED display 1306 pixel size 128x64, 0.96 inch
 * Interface I2C
 * Important! Define SSD1306_128_64 in Adafruit_SSD1306_STM32.h
 * 
 * Wiring: SCLK  .. B6
 *         SDA   .. B7
 *         
 *         
 */

#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <FreeMono9pt7b.h>

//------------

void setup() {
    //Define main states robot can have
    enum mainStates{lineFollowing,avengerCollecting,avengerScoring,stoneCollecting,stoneScoring,defending} mainState;
    mainState = lineFollowing;

    //Declare State Classes
    //StateLoops::LineFollow lineFollow;
    //StateLoops::AvengerCollect avengerCollect;
    //StateLoops::AvengerScore avengerScore;
    //StateLoops::StoneCollect stoneCollect;
    //StateLoops::StoneScore stoneScore;
    //StateLoops::Defend defend;

    //Hardware::QRD qrd1;
    //Hardware::Motor leftMotor;

    for(;;) {
      switch(mainState){
        case(lineFollowing):
          //lineFollow.loop(qrd1);
          break;
        case(avengerCollecting):
          //avengerCollect.loop();
          break;
        case(avengerScoring):
          //avengerScore.loop();
          break;
        case(stoneCollecting):
          //stoneCollect.loop();
          break;
        case(stoneScoring):
          //stoneScore.loop();
          break;
        case(defending):
          //defend.loop();
          break;
        default:
          break;
      }
    }
}

void loop() {}
