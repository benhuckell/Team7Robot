#include "Hardware/Encoder.h"
  
Encoder::Encoder(int encoder_port_1, int encoder_port_2, int encoder_orientation_local) {
   Encoder::encoder_port_1 = encoder_port_1;
   Encoder::encoder_port_2 = encoder_port_2;
   lastTime = millis();
   count = 0;
   lastCount = 0;
   speed = 0;
   encoder_orientation = encoder_orientation_local;
  
}

//gets called on every rising edge of the encoder output
void Encoder::ISR(){
   dir = digitalRead(encoder_port_2);
   //Serial.print("encoder Port1: "+ String (encoder_port_1) + " | ");
   //Serial.println("encoder Port2: "+ String (encoder_port_2));
   //Serial.println("dir: "+ String( dir));
   if(dir > 0){
       count = count + (encoder_orientation); // counts 1 up (allegedly)
       //Serial.println("+1 : " + String(count));
   }
   else{
       count = count - (encoder_orientation); // counts 1 down (allegedly)
       //Serial.println("-1 " + String(count));
   }
}

void Encoder::ISR_winch(){
    digitalWrite(PA15, HIGH);
     dir = winch_dir;
   //Serial.print("encoder Port1: "+ String (encoder_port_1) + " | ");
   //Serial.println("encoder Port2: "+ String (encoder_port_2));
   //Serial.println("dir: "+ String( dir));
   if(dir > 0){
       count = count + (encoder_orientation); // counts 1 up (allegedly)
       //Serial.println("+1 : " + String(count));
   }
   else{
       count = count - (encoder_orientation); // counts 1 down (allegedly)
       //Serial.println("-1 " + String(count));
   }
   digitalWrite(PA15, LOW);
}

void Encoder::update(){
   int elapsedTime = millis() - lastTime;
   lastTime = millis();

   if(dir == HIGH){
       direction = 1;
   }else if(dir == LOW){
       direction = -1;
   }
  
   int ticks = count - lastCount;
   lastCount = count;
   speed = (float)1000*ticks/elapsedTime*direction;
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
