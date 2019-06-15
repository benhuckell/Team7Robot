#include "Arduino.h"
#include "StateLoops/lineFollow.h"


namespace StateLoops {
    
    enum lineFollowStates{online,offline} lineFollowState;
    
    void LineFollow::loop(){

        //qrd1.getValue()
        
        //motor1.value = 50;
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