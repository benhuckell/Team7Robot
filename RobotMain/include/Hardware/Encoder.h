#pragma once
#include "Arduino.h"


class Encoder {
   public:
       Encoder(int encoder_port_1, int encoder_port_2, int encoder_orientation, float scaleFactor);
       void ISR();
       float getSpeed();
       int getCount();
       void resetCount(); //Be Careful!!!
       void update();

       void ISR_winch();
       volatile int winch_dir = -0;
       volatile int count1_dir = 1;
       volatile int count2_dir = 1;
       void ISRCountAve1();
       void ISRCountAve2();
      
       const float ticksPerRotation = 90;
   private:
       int encoder_port_1;
       int encoder_port_2;
       unsigned int lastTime;
       volatile int count;
       int lastCount;
       float speed;
       int dir;
       int direction;
       int encoder_orientation;
       float scaleFactor;

       volatile int count1;
       volatile int count2;
};
