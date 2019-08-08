#include "Hardware/Encoder.h"
  
Encoder::Encoder(int encoder_port_1, int encoder_port_2, int encoder_orientation_local, float scaleFactor_local) {
   Encoder::encoder_port_1 = encoder_port_1;
   Encoder::encoder_port_2 = encoder_port_2;
   lastTime = millis();
   count = 0;
   lastCount = 0;
   speed = 0;
   encoder_orientation = encoder_orientation_local;
   scaleFactor = scaleFactor_local;
  
}

//gets called on every rising edge of the encoder output
void Encoder::ISR(){
   //digitalWrite(PB9, HIGH);
   dir = digitalRead(encoder_port_2);
   if(dir > 0){
       count = count + (encoder_orientation); // counts 1 up (allegedly)
   }
   else{
       count = count - (encoder_orientation); // counts 1 down (allegedly)
   }
   //digitalWrite(PB9, LOW);
}

void Encoder::ISRCountAve1(){
          //digitalWrite(PA15, HIGH);
    dir = count1_dir;
   if(dir > 0){
       count1 = count1 + (encoder_orientation); // counts 1 up (allegedly)
   }
   else{
       count1 = count1 - (encoder_orientation); // counts 1 down (allegedly)
   }
   //digitalWrite(PA15, LOW);
}

void Encoder::ISRCountAve2(){
   //digitalWrite(PA15, HIGH);
    dir = count2_dir;
   if(dir > 0){
       count2 = count2 + (encoder_orientation); // counts 1 up (allegedly)
   }
   else{
       count2 = count2 - (encoder_orientation); // counts 1 down (allegedly)
   }
   //digitalWrite(PA15, LOW);
}

void Encoder::ISR_winch(){
    //digitalWrite(PA15, HIGH);
    dir = winch_dir;

   if(dir > 0){
       count = count + (encoder_orientation); // counts 1 up (allegedly)
   }
   else{
       count = count - (encoder_orientation); // counts 1 down (allegedly)
   }
   //digitalWrite(PA15, LOW);
}


void Encoder::update(){
   int elapsedTime = millis() - lastTime;
   lastTime = millis();
  
   float ticks = float(count)*scaleFactor - float(lastCount);
   lastCount = int(count*scaleFactor);
   speed = float(1000*(float(ticks)/float(elapsedTime)));
}

float Encoder::getSpeed() {
   return speed;
}

int Encoder::getCount() {
   return int(count*scaleFactor);
}


void Encoder::resetCount() {
   count = 0;
}

// void Encoder::getCountAve(){
//    return count1 + count2 / 2;
// }