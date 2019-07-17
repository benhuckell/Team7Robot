#include "StateLoops/avengerScore.h"
#include "stateController.h"

using namespace StateLoops;

//enum avengerScoreStates{} avengerScoreState;

AvengerScore::AvengerScore(){
    AvengerScore::HI = HardwareInterface::i();
}

void AvengerScore::loop(){
    MainState::instance()->setState(stoneCollecting);
    display.println("Avenger Scoring");
    delay(1000);
    return;
}

void AvengerScore::goToBeacon() {

}
void AvengerScore::dumpAvengers() {

}