#include "Hardware/DriveMotor.h"

DriveMotor::DriveMotor(PinName forward, PinName back){
    DriveMotor::motor_port_forwards = forward;
    DriveMotor::motor_port_backwards = back;

    T_PWM = 500; //500 clock pulses
    PWM_FREQ = 100000; //100KHz 
    
    pinMode(motor_port_forwards, OUTPUT);
    pinMode(motor_port_backwards, OUTPUT);

    pwm_start(motor_port_forwards, PWM_FREQ, T_PWM, 0, 1);
    pwm_start(motor_port_backwards, PWM_FREQ, T_PWM, 0, 1);
}

void DriveMotor::update(){
    //limit power to |100|
    //pwm_start(PA_2, 100000, 500, 250, 0);
    if(DriveMotor::speed > 100){
        DriveMotor::speed = 100;
    } else if(DriveMotor::speed < -100){
        DriveMotor::speed = -100;
    }
    float t_pulse = DriveMotor::T_PWM * (float)DriveMotor::speed/100.0;
    if(t_pulse < 0){
        t_pulse *= -1;
    }
    if(DriveMotor::speed >= 0){
        //fowards
        pwm_start(motor_port_forwards, PWM_FREQ, T_PWM, t_pulse, 0);
        pwm_start(motor_port_backwards, PWM_FREQ, T_PWM, 0, 0);
    }
    else if(DriveMotor::speed < 0){
        //backwards
        pwm_start(motor_port_forwards, PWM_FREQ, T_PWM, 0, 0);
        pwm_start(motor_port_backwards, PWM_FREQ, T_PWM, t_pulse, 0);
    }
}

int DriveMotor::getSpeed(){
    return DriveMotor::speed;
}

void DriveMotor::setSpeed(int speed){
    DriveMotor::speed = speed;
}