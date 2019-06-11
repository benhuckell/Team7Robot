#include "Arduino.h"
#include "StateLoops/MainStates.h"
#include <iostream>

namespace StateLoops {
    
    enum lineFollowStates{online,offline} lineFollowState;
    
    void LineFollow::loop(){
        /*
        driveTrain.speed = 50
        driveTrain.direction = back
        startPIDLoop()
        driveTrain.setMotorSpeed()
        */
    }

    void LineFollow::findIR() {

    }

    void LineFollow::findPost() {

    }

    void LineFollow::findGauntlet() {

    }

    void LineFollow::findLine() {

    }
}