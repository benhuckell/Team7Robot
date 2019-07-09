#pragma once

#include "Arduino.h"

class DriveMotor {
   public:
      DriveMotor(PinName motor_port_forwards, PinName motor_port_backwards);
      void update();
      int getSpeed();
      void setSpeed(int speed);
      
   private:
      int speed;
      PinName motor_port_forwards;
      PinName motor_port_backwards;
      
      int T_PWM; //2000 clock pulses
      int PWM_FREQ; //100KHz
};