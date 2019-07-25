#include "StateLoops/stoneScore.h"
#include "stateController.h"

using namespace StateLoops;

StoneScore::StoneScore(){
    StoneScore::HI = HardwareInterface::i();
}

void StoneScore::loop(){
    display.println("Stone Scoring");
    delay(1000);
    goToHole(holeOne);
    placeStone();
}

void StoneScore::goToHole(HoleNumbers holeNum) {
    //back up slightly
    HI->LMotor->setSpeed(-30);
    HI->RMotor->setSpeed(-30);
    HI->LMotor->update();
    HI->RMotor->update();
    delay(500);
    HI->LMotor->setSpeed(0);
    HI->RMotor->setSpeed(0);
    HI->LMotor->update();
    HI->RMotor->update();

    //turn to hole
    HI->turn_time(holeTurnAngles[holeNum],2000);

    //go forwards
    HI->LMotor->setSpeed(30);
    HI->RMotor->setSpeed(30);
    HI->LMotor->update();
    HI->RMotor->update();
    delay(500);
    HI->LMotor->setSpeed(0);
    HI->RMotor->setSpeed(0);
    HI->LMotor->update();
    HI->RMotor->update();

    //select which motor to turn with
    int motor = 1;
    if(holeNum > 1){
        motor = -1;
    }

    HI->turn_single(holeTurnAngles[holeNum]*-1, motor, 1/*forwards*/, 2000);
}

void StoneScore::placeStone() {
    
}

void StoneScore::returnToTape() {
    //back up slightly
    HI->LMotor->setSpeed(-30);
    HI->RMotor->setSpeed(-30);
    HI->LMotor->update();
    HI->RMotor->update();
    delay(500);
    HI->LMotor->setSpeed(0);
    HI->RMotor->setSpeed(0);
    HI->LMotor->update();
    HI->RMotor->update();

    //turn to Line
    HI->turnOnLine();
}