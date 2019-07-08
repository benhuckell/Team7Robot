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

}

void StoneScore::returnToTape() {

}