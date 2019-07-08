#include "StateLoops/avengerCollect.h"
#include "Arduino.h"
#include "configGlobals.h"

namespace StateLoops {

    enum avengerCollectStates{} avengerCollectState;

    void AvengerCollect::loop(){
        //Main Code
        display.println("Avenger Collecting");
        delay(1000);
        //State Change
        return;
    }
    
    void AvengerCollect::intakeAvengers() {

    }
}