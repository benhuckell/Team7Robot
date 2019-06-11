#include "Arduino.h"
#include <iostream>
#include "StateLoops/MainStates.h"

void setup()
{
    //Define main states robot can have
    enum mainStates{lineFollowing,avengerCollecting,avengerScoring,stoneCollecting,stoneScoring,defending} mainState;
    mainState = lineFollowing;

    //Declare State Classes
    StateLoops::LineFollow lineFollow;
    StateLoops::AvengerCollect avengerCollect;
    StateLoops::AvengerScore avengerScore;
    StateLoops::StoneCollect stoneCollect;
    StateLoops::StoneScore stoneScore;
    StateLoops::Defend defend;

    for(;;){
      switch(mainState){
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

      driveTrain.tick()
    }
}

void loop() {}
