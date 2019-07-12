#include "Hardware/DriveMotor.h"

DriveMotor::DriveMotor(PinName motor_port_forwards, PinName motor_port_backwards){
    DriveMotor::motor_port_forwards = motor_port_forwards;
    DriveMotor::motor_port_backwards = motor_port_forwards;

    DriveMotor::T_PWM = 2000; //2000 clock pulses
    DriveMotor::PWM_FREQ = 100000; //100KHz 
}

void DriveMotor::update(){
    //limit power to 100
    if(DriveMotor::speed > 100){
        DriveMotor::speed = 100;
    }
    //Left Motor
    int t_pulse = DriveMotor::T_PWM * ( 1.0 - DriveMotor::speed/100);
    if(DriveMotor::speed >= 0){
        //fowards
        pwm_start(DriveMotor::motor_port_forwards, PWM_FREQ, T_PWM, int(t_pulse), 1);
        pwm_start(DriveMotor::motor_port_backwards, PWM_FREQ, T_PWM, 0, 1);
    }
    else if(DriveMotor::speed < 0){
        //backwards
        pwm_start(DriveMotor::motor_port_forwards, PWM_FREQ, T_PWM, 0, 1);
        pwm_start(DriveMotor::motor_port_backwards, PWM_FREQ, T_PWM, int(t_pulse), 1);
    }
}

int DriveMotor::getSpeed(){
    return DriveMotor::speed;
}

void DriveMotor::setSpeed(int speed){
    DriveMotor::speed = speed;
}