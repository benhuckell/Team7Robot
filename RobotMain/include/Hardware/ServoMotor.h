#pragma once
#include "Arduino.h"
#include <ctime>

class ServoMotor {
   public:
      ServoMotor(PinName motor_port);
      void update();
      void clawSetPos(int clawAngle);
      
   private:
      PinName motor_port;
      bool openClaw;
      const int openAngle = 300;
      const int closedAngle = 200;
};
