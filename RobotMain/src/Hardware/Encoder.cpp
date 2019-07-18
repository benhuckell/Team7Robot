#include "Hardware/Encoder.h"
    
Encoder::Encoder(int encoder_port_1, int encoder_port_2) {
    Encoder::encoder_port_1 = encoder_port_1;
    Encoder::encoder_port_2 = encoder_port_2;
    lastTime = millis();
    count = 0;
    lastCount = 0;
    speed = 0;
}

//gets called on every rising edge of the encoder output
void Encoder::ISR(){
    count++;
    dir = digitalRead(encoder_port_2);
}


void Encoder::update(){
    int elapsedTime = millis() - lastTime;
    lastTime = millis();

    if(dir == LOW){
        direction = -1;
    }else if(dir == HIGH){
        direction = 1;
    }
    
    int ticks = count - lastCount; 
    lastCount = count;
 
    speed = ticks/elapsedTime*direction;
}

int Encoder::getSpeed() {
    return speed;
}

int Encoder::getCount() {
    return count;
}


void Encoder::resetCount() {
    count = 0;
}
