#pragma once
#include "Arduino.h"
#include <ctime>

class ServoMotor {
   public:
      ServoMotor(PinName motor_port, int startingAngle);
      void update();
      void setPos(int angle);
      int getPos();
   private:
      PinName motor_port;
      int currentAngle;
};