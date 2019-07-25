#include "StateLoops/stoneScore.h"
#include "stateController.h"


using namespace StateLoops;
enum stoneScoreStates{} stoneScoreState;
enum holeNumbers{holeOne,holeTwo,holeThree,holeFour,holeFive} holeNumber;

StoneScore::StoneScore(){
    StoneScore::HI = HardwareInterface::i();
}

void StoneScore::loop(){
    display.println("Stone Scoring");
}

void StoneScore::goToGauntlet() {

}

void StoneScore::scoreStrat(){
    if(HI->checkForRock()){
        goToGauntlet();
    }
    else{
        //go back to line follow sequence
    }
}

void StoneScore::findHole(enum holeNumbers) {

}

void StoneScore::placeStone() {
    //requires moving down with the winch to the correct height
    //limit switch should no longer be pressed
    //return winch to the bottom of the rod, for being manipulated for the next stone collect

}

void StoneScore::returnToTape() {
    MainState::instance()->setState(lineFollowing);
}