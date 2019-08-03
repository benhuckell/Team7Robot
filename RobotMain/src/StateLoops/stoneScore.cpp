#include "StateLoops/stoneScore.h"
#include "stateController.h"


using namespace StateLoops;
enum stoneScoreStates{} stoneScoreState;
enum holeNumbers{holeOne,holeTwo,holeThree,holeFour,holeFive} holeNumber;

void StoneScore::loop(){
    MainState::instance()->setState(defending);
    display.println("Stone Scoring");
    delay(1000);
    return;
}

void StoneScore::goToGauntlet() {

}

void StoneScore::findHole(enum holeNumbers) {

}

void StoneScore::placeStone() {
    //  HI-> winchTickTarget = 10;
    //  HI->WinchEncoder->winch_dir=-1;

    //     //raise intake
    //     while(HI->winchTickTarget - HI->WinchEncoder->getCount() > 5){
    //         HI->moveIntake_const_speed();
    //         Serial.println("en: " + String(HI->WinchEncoder->getCount()));
    //         Serial.println("winch dir: " + String(HI->WinchEncoder->winch_dir));
    //     }

    //     //closing the claw around the rock
    //     HI->clawMotor->clawSetPos(300);
    //     delay(1000);
        
    //     HI->WinchEncoder->winch_dir=1;
    //     HI-> winchTickTarget=270;

    //     //lifting up to make sure rock isn't still in the pole mount
    //     while(HI->winchTickTarget - HI->WinchEncoder->getCount() > 5){
    //         HI-> moveIntake_const_speed();
    //         Serial.println("en: " + String(HI->WinchEncoder->getCount()));
    //         Serial.println("winch dir: " + String(HI->WinchEncoder->winch_dir));
        //}

}

void StoneScore::returnToTape() {

}

