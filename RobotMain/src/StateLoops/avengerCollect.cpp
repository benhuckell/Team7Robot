#include "StateLoops/avengerCollect.h"
#include "Arduino.h"
#include "configGlobals.h"

namespace StateLoops {

    enum avengerCollectStates{} avengerCollectState;

    void AvengerCollect::loop(){
        MainState::i()->setState(avengerScoring);
        //Main Code
        display.println("Avenger Collecting");
        delay(1000);
        //State Change
        return;
    }
    
    void AvengerCollect::intakeAvengers() {

    }
}