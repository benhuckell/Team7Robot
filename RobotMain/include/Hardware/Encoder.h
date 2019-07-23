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
        
        const float ticksPerRotation = 90;
    private:
        int encoder_port_1;
        int encoder_port_2;
        unsigned int lastTime;
        int count;
        int lastCount;
        float speed;
        int dir;
        int direction;

};