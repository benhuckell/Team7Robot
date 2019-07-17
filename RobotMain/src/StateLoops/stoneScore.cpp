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

void StoneScore::findHole(enum holeNumbers) {

}

void StoneScore::placeStone() {

}

void StoneScore::returnToTape() {
    MainState::instance()->setState(lineFollowing);
}