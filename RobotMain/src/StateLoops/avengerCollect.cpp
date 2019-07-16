#include "StateLoops/avengerCollect.h"
#include "stateController.h"

using namespace StateLoops;

enum avengerCollectStates{} avengerCollectState;

void AvengerCollect::loop(){
    MainState::instance()->setState(avengerScoring);
    //Main Code
    display.println("Avenger Collecting");
    //State Change
    return;
}

void AvengerCollect::intakeAvengers() {

}
