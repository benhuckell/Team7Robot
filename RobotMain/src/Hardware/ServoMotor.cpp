#include "Hardware/ServoMotor.h"

ServoMotor::ServoMotor(PinName port){
    ServoMotor::motor_port = port;
    ServoMotor::openClaw = true;
    pwm_start(motor_port, 100000, 500, 0, 1); //init
}

//update motor speed and position
void ServoMotor::clawSetPos(int clawAngle) {
    pwm_start(motor_port, 100000, 500, clawAngle, 0);
}
