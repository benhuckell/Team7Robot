#pragma once
#include "Arduino.h"

class Encoder {
    public:
        Encoder(int encoder_port_1, int encoder_port_2);
        void ISR();
        int getSpeed(); 
        int getCount();
        void resetCount(); //Be Careful!!!
        void update();

    private:
        int encoder_port_1;
        int encoder_port_2;
        unsigned int lastTime;
        int count;
        int lastCount;
        float speed;
        int dir;
        int direction;
        const float WheelDiameter = 0.02; //metres
        const float ticksPerRotation = 90;

};