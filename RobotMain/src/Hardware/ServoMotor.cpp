#include "Hardware/ServoMotor.h"

ServoMotor::ServoMotor(PinName port, int startingAngle){
    ServoMotor::motor_port = port;
    pwm_start(motor_port, 100000, 500, 0, 1); //init
    setPos(startingAngle);
}

void ServoMotor::setPos(int angle) {
    pwm_start(motor_port, 100000, 500, angle, 0);
    currentAngle = angle;
}

int ServoMotor::getPos(){
    return currentAngle;
}