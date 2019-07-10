#include "StateLoops/avengerCollect.h"
#include "Arduino.h"
#include "stateController.h"

namespace StateLoops {

    enum avengerCollectStates{} avengerCollectState;

    void AvengerCollect::loop(){
        //Main Code
        display.println("Avenger Collecting");
        //State Change
        return;
    }
    
    void AvengerCollect::intakeAvengers() {

    }
}