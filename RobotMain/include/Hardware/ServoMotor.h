#pragma once
#include "Arduino.h"
#include <ctime>

class ServoMotor {
   public:
      ServoMotor(PinName motor_port);
      void update();
      
   private:
      PinName motor_port;
      int speed;
      int position;
};
