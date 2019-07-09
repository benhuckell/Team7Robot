#include "StateLoops/stoneScore.h"
#include "Arduino.h"
#include "stateController.h"

namespace StateLoops {
    enum stoneScoreStates{} stoneScoreState;
    enum holeNumbers{holeOne,holeTwo,holeThree,holeFour,holeFive} holeNumber;
    
    void StoneScore::loop(){
        MainState::i()->setState(defending);
        display.println("Stone Scoring");
        delay(1000);
        return;
    }

    void StoneScore::goToGauntlet() {
    
    }

    void StoneScore::findHole(enum holeNumbers) {

    }

    void StoneScore::placeStone() {

    }

    void StoneScore::returnToTape() {

    }
}