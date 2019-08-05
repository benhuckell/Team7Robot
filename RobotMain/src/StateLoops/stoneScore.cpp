#include "StateLoops/stoneScore.h"
#include "stateController.h"

enum stoneScoreStates{} stoneScoreState;
enum holeNumbers{holeOne,holeTwo,holeThree,holeFour,holeFive} holeNumber;

namespace StateLoops{
    StoneScore::StoneScore(){
        StoneScore::HI = HardwareInterface::i();
    }

    void StoneScore::loop(){
        display.println("Stone Scoring");

        //Drop Stone
        HI->clawSetPos(100);
        delay(1000);
        HI->QRDTurnNew(false);
        delay(200);
        HI->moveIntake_const_speed();
        MainState::instance()->setState(lineFollowing);

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