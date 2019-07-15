#include "Hardware/DriveMotor.h"

DriveMotor::DriveMotor(PinName forward, PinName back){
    DriveMotor::motor_port_forwards = forward;
    DriveMotor::motor_port_backwards = back;
    Serial.print(motor_port_backwards);
    Serial.print(motor_port_forwards);

    T_PWM = 500; //500 clock pulses
    PWM_FREQ = 100000; //100KHz 
    pwm_start(motor_port_forwards, PWM_FREQ, T_PWM, 0, 1);
    pwm_start(motor_port_backwards, PWM_FREQ, T_PWM, 0, 1);

    pinMode(motor_port_forwards, OUTPUT);
    pinMode(motor_port_backwards, OUTPUT);
}

void DriveMotor::update(){
    //limit power to 100
    pwm_start(PA_0, 100000, 500, 250, 0);
    if(DriveMotor::speed > 100){
        DriveMotor::speed = 100;
    } else if(DriveMotor::speed < -100){
        DriveMotor::speed = -100;
    }
    //Left Motor
    int t_pulse = DriveMotor::T_PWM * ( 1.0 - DriveMotor::speed/100);
    if(DriveMotor::speed >= 0){
        //fowards
        pwm_start(motor_port_forwards, PWM_FREQ, T_PWM, 250, 0);
        pwm_start(motor_port_backwards, PWM_FREQ, T_PWM, 0, 0);
    }
    else if(DriveMotor::speed < 0){
        //backwards
        pwm_start(motor_port_forwards, PWM_FREQ, T_PWM, 0, 0);
        pwm_start(motor_port_backwards, PWM_FREQ, T_PWM, 250, 0);
    }
}

int DriveMotor::getSpeed(){
    return DriveMotor::speed;
}

void DriveMotor::setSpeed(int speed){
    DriveMotor::speed = speed;
}