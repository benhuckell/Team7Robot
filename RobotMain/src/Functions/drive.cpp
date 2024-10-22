#include "Functions/drive.h"
#include "Hardware/HardwareInterface.h"
#include "Functions/lineFollow.h"
#include <queue>
//#include "stateController.h"

void drive_stop_seq(int direction, int deadtime, int timeout, float delta_trip, float kdrift, int maxpower){
    HardwareInterface* HI = HardwareInterface::i();
    
    //must set 
    //delta trip is speed of encoder to trip

    int drift = 0;
    float ave_speed = 0.0;
    int totalDriftError = 0;

    int Lpower = 0;
    int Rpower = 0;

    int local_start_tic_L = 0;
    int local_start_tic_R = 0;
    int start_time = millis();

    int function_time_start = millis();
    int function_net_time = millis();

    while(millis() - start_time < deadtime){
        local_start_tic_L = HI->LEncoder->getCount();  
        local_start_tic_R = HI->REncoder->getCount();  
        function_net_time = millis() - function_time_start;

        Serial.println("countL : " + String(HI->LEncoder->getCount()));
        Serial.println("countR: " + String(HI->REncoder->getCount()));
        delay(30);
        function_time_start = millis();
        drift = (HI->LEncoder->getCount() - local_start_tic_L) - (HI->REncoder->getCount() - local_start_tic_R);
        totalDriftError += drift;
        ave_speed = (abs(HI->LEncoder->getSpeed()) + abs(HI->REncoder->getSpeed()))/2.0;

        Lpower = (direction*maxpower) - (totalDriftError * kdrift);
        Rpower = ((direction*maxpower)/straightLineCorrectionFactor) + (totalDriftError * kdrift);

        HI->pushDriveSpeeds(Lpower, Rpower);
        Serial.println("ave speed" + String(ave_speed));
        Serial.println("Lspeed: " + String(Lpower));
        Serial.println("Rspeed: " + String(Rpower));
        Serial.println("dridf: "+ String(drift));
        Serial.println("Total Drift Error: " + String(totalDriftError));
        Serial.println("function time: " + String(function_net_time));

        Serial.println("");
        HI->update();
    }
        Serial.println("drive_stop: end of deadtime: " + String(ave_speed));

    while(true){

      if (ave_speed < delta_trip) {
        
        Serial.println("EXIT: speed");
        break;
      }
      if ((millis()-start_time) > timeout){
          Serial.println("EXIT: time");
        break;
      }
          local_start_tic_L = HI->LEncoder->getCount();  
          local_start_tic_R = HI->REncoder->getCount();  

          delay(30);
          drift = (HI->LEncoder->getCount() - local_start_tic_L) - (HI->REncoder->getCount() - local_start_tic_R);
          totalDriftError += drift;

          ave_speed = (abs(HI->LEncoder->getSpeed()) + abs(HI->REncoder->getSpeed()))/2.0;

          Lpower = (direction*maxpower) - (totalDriftError * kdrift);
          Rpower = ((direction*maxpower)/straightLineCorrectionFactor) + (totalDriftError * kdrift);

          HI->pushDriveSpeeds(Lpower, Rpower);
          Serial.println("ave speed" + String(ave_speed));
          Serial.println("Lspeed: " + String(Lpower));
          Serial.println("Rspeed: " + String(Rpower));
          Serial.println("dridf: "+ String(drift));
          Serial.println("Total Drift Error: " + String(totalDriftError));
          Serial.println("");
        HI->update();
      }
    Serial.println("exit reached!");
    Serial.println("Total Drift Error: " + String(totalDriftError));

    HI->pushDriveSpeeds(-33, -33/straightLineCorrectionFactor);
    delay(110);
    HI->pushDriveSpeeds(0, 0);

  }

void drive_stop(int powerL, int powerR, int powerL_dead, int powerR_dead, int deadtime, int timeout, float delta_trip, int backSpeed){
    HardwareInterface* HI = HardwareInterface::i();
    
    //must set 
    //delta trip is speed of encoder to trip
    float ave_speed = 0.0;
    int start_time = millis();
    HI->pushDriveSpeeds(powerL_dead, powerR_dead);
    while(millis() - start_time < deadtime){
        ave_speed = (abs(HI->LEncoder->getSpeed()) + abs(HI->REncoder->getSpeed()))/2.0;
        Serial.println("ave speed" + String(ave_speed));
        Serial.println("");
        HI->update();
        delay(30);
    }
    HI->pushDriveSpeeds(powerL, powerR);
        Serial.println("drive_stop: end of deadtime: " + String(ave_speed));
      //delay(10000000);
    while(true){

      if (ave_speed < delta_trip) {
        
        Serial.println("EXIT: speed");
        break;
      }
      if ((millis()-start_time) > timeout){
          Serial.println("EXIT: time");
        break;
      }

          ave_speed = (abs(HI->LEncoder->getSpeed()) + abs(HI->REncoder->getSpeed()))/2.0;

          Serial.println("ave speed" + String(ave_speed));
          HI->update();
          delay(30);
      }
    Serial.println("exit reached!");
    
    HI->pushDriveSpeeds(-30, -33/1.14);
    delay(110);
    HI->pushDriveSpeeds(0, 0);


  }



void stopMoving(){
    HardwareInterface* HI = HardwareInterface::i(); 
    HI->pushDriveSpeeds(-37, -35/1.16);
    delay(200);
    HI->pushDriveSpeeds(0, 0);
    delay(150);
}


void stopMoving_Post1(){
    HardwareInterface* HI = HardwareInterface::i(); 
    HI->pushDriveSpeeds(0, 0);
    delay(70);
    HI->pushDriveSpeeds(-35, -37/straightLineCorrectionFactor);
    delay(200);
    HI->pushDriveSpeeds(0, 0);
    delay(150);
}


void jdubDrive(int direction, int target, int maxpower, int minpower, unsigned int timeout, float kaccel, float kdeaccel, float kdrift){
  HardwareInterface* HI = HardwareInterface::i(); 

  int netpower = maxpower - minpower;
  float boost;
  int brakepower = 15;

  int Ltics_start = HI->LEncoder->getCount();
  int Rtics_start = HI->REncoder->getCount();

  int encoderaverage = 0;
  int start_time = millis();
  unsigned int net_time = 0;

  while( (encoderaverage < target) && (net_time < timeout)){

    net_time  = millis() - start_time;

    int Ltics = abs(HI->LEncoder->getCount() - Ltics_start); 
    int Rtics = abs(HI->REncoder->getCount() - Rtics_start); 
    encoderaverage = (Ltics+Rtics) /2;
    int encoder_error = Ltics - Rtics; //subtract from L side

    //accel/deaccel constants
    if(encoderaverage < target*kaccel){
      boost = (encoderaverage)/(target*kaccel);
    Serial.println("accelb");
      }
    else if (encoderaverage > (target - target*kdeaccel)){
    Serial.println("deaccelb");
      boost = (target-encoderaverage)/(target - target*kdeaccel);}
    else{
      boost = 1;}

    int power_L = direction*HI->motorcap(minpower + boost*netpower, 100, 0) - (direction*encoder_error*kdrift);
    int power_R = direction*HI->motorcap(minpower + boost*netpower, 100, 0) + (direction*encoder_error*kdrift);
    
    HI->pushDriveSpeeds(power_L, power_R/straightLineCorrectionFactor);

    //Serial.println("Ltics: " + String(Ltics));
    //Serial.println("Rtics: " + String(Rtics));
    //Serial.println("enError: " + String(encoder_error));
    //Serial.println("boost: " + String(boost));
    //Serial.println("powL: " + String(power_L));
    //Serial.println("powR: " + String(power_R));
    //Serial.println("");

    delay(20);
    }
  //brake
  HI->pushDriveSpeeds(-direction*brakepower, -direction*brakepower/straightLineCorrectionFactor);

  }

//turn with QRD at end
//choose whether to follow tape before or not
void QRDTurn(bool rightTurn, int deadtime,int powerL, int powerR, bool followTapeVar, int followTapeDuration){
    HardwareInterface* HI = HardwareInterface::i(); 

    if(followTapeVar){
      int startTime = millis();
      while(millis()-startTime < followTapeDuration){
          followTape(30, !rightTurn, true);
          HI->update();
          }
      stopMoving();
      Serial.println("Tape follow extra end");
    }

    int speedL;
    int speedR;
  
    speedL = powerL;
    speedR = powerR;
    
    Serial.println("QRD turn : left case");
    int start_time = millis();
    while(millis() - start_time < deadtime){
        HI->pushDriveSpeeds(speedL, speedR);
    }
    HI->update();
    Serial.println("QRD3i: " + String(HI->QRD_Vals[3]));
    Serial.println("QRD4i: " + String(HI->QRD_Vals[4]));
    Serial.println("Forced turn end");
    while(HI->QRD_Vals[3] < 0.8 && HI->QRD_Vals[4] < 0.8){
        
         HI->pushDriveSpeeds(speedL, speedR);
         HI->update();
        //Serial.println("QRD3: " + String(HI->QRD_Vals[3]));
        //Serial.println("QRD4: " + String(HI->QRD_Vals[4]));
    }
    Serial.println("detected!");
    HI->pushDriveSpeeds(-speedL/2, -speedR/2);
    delay(400);
    HI->pushDriveSpeeds(0, 0);
    delay(500);
}




void turn_single_constant(int target, unsigned int timeout, int robotSpeed){
  //pos, left back, left turn
  //neg 
    HardwareInterface* HI = HardwareInterface::i(); 
    int startTime = millis();
    HI->update();
    int LStartCount = HI->LEncoder->getCount();
    int RStartCount = HI->REncoder->getCount();
    while(millis() - startTime <= timeout){
      Serial.println("LCount: " + String(HI->LEncoder->getCount() - LStartCount));
      Serial.println("RCount: " + String(HI->REncoder->getCount() - RStartCount));
      Serial.println("");
        if(target >= 0){
            HI->pushDriveSpeeds(0, -robotSpeed);
            HI->update();
            if(HI->REncoder->getCount() - RStartCount <= -target){
                HI->pushDriveSpeeds(0, 0);
                HI->update();
                return;
            }
        }
        else if(target < 0){

            HI->pushDriveSpeeds(-robotSpeed, 0);
            HI->update();
            //Serial.println("turn LCount net: " + String(HI->LEncoder->getCount() - LStartCount));
            //Serial.println("turn Motor speed: " + String(robotSpeed));
            //Serial.println(" LCount: " + String(HI->LEncoder->getCount()));
            //Serial.print(HI->LEncoder->getCount());
            //Serial.print(target);
            //Serial.print(" LStartCount: ");
            //Serial.print(LStartCount);
            if(HI->LEncoder->getCount() - LStartCount <= target){
                Serial.println("hit");
                HI->pushDriveSpeeds(0, 0);
                HI->update();
                return;
            }
        }
    }
    HI->pushDriveSpeeds(0, 0);
    HI->update();
  }


void turn_accel(int target, int boost, int boost_time, int power, int timeout){

  HardwareInterface* HI = HardwareInterface::i(); 
    int startTime = millis();
    HI->update();
    int LStartCount = HI->LEncoder->getCount();
    int RStartCount = HI->REncoder->getCount();
    
    if(target>=0){
      //right turn, right motor back
      HI->pushDriveSpeeds(0, -boost);
      while(millis() - startTime < boost_time){
        delay(10);
      }
      HI->pushDriveSpeeds(0, -power);
      while((millis()-startTime < timeout)){
        if((HI->REncoder->getCount() - RStartCount <= -target) ){
          break;
        }
         delay(10);
      }
      HI->pushDriveSpeeds(0, 0);
      HI->update();
              
    }

    else{
      //left turn, left motor back
      HI->pushDriveSpeeds(-boost,0);
      while(millis() - startTime < boost_time){
        delay(10);
      }
      HI->pushDriveSpeeds(-power,0);
      while((millis()-startTime < timeout)){
        if((HI->REncoder->getCount() - RStartCount <= -target)){
          break;
        }
         delay(10);
      }
      HI->pushDriveSpeeds(0, 0);
      HI->update();
            
    }

}


  
// void turn_single_constant_time(int target, unsigned int timeout, int robotSpeed){
//     HardwareInterface* HI = HardwareInterface::i(); 
//     int startTime = millis();
//     HI->update();
//     int LStartCount = HI->LEncoder->getCount();
//     int RStartCount = HI->REncoder->getCount();
//     while(millis() - startTime <= target){
//         if(target >= 0){
//             HI->pushDriveSpeeds(0, -robotSpeed/straightLineCorrectionFactor);
//             HI->update();
//             if(HI->REncoder->getCount() - RStartCount <= -target){
//                 HI->pushDriveSpeeds(0, 0);
//                 HI->update();
//                 return;
//             }
//         }
//         else if(target < 0){

//             HI->pushDriveSpeeds(-robotSpeed, 0);
//             HI->update();
//             //Serial.println("turn LCount net: " + String(HI->LEncoder->getCount() - LStartCount));
//             //Serial.println("turn Motor speed: " + String(robotSpeed));
//             //Serial.println(" LCount: " + String(HI->LEncoder->getCount()));
//             //Serial.print(HI->LEncoder->getCount());
//             //Serial.print(target);
//             //Serial.print(" LStartCount: ");
//             //Serial.print(LStartCount);
//             if(HI->LEncoder->getCount() - LStartCount <= target){
//                 Serial.println("hit");
//                 HI->pushDriveSpeeds(0, 0);
//                 HI->update();
//                 return;
//             }
//         }
//     }
//     HI->pushDriveSpeeds(0, 0);
//     HI->update();
//   }

void jiggle(){
    HardwareInterface* HI = HardwareInterface::i();
    HI->pushDriveSpeeds(0, 60/1.13);
    delay(100);
     HI->pushDriveSpeeds(60, 0);
    delay(100);
       HI->pushDriveSpeeds(0, 60/1.13);
    delay(100);
     HI->pushDriveSpeeds(60, 0);
    delay(100);
       HI->pushDriveSpeeds(0, 0);
    delay(100);
    
}


void time_turn(bool left, int power, int timeout, int brakepower){
  //
    HardwareInterface* HI = HardwareInterface::i(); 
    int startTime = millis();
    HI->update();
    int dir = 0;

    if (left){
      HI->pushDriveSpeeds(-power, 0);
    }
    else{
      HI->pushDriveSpeeds(0, -power);
    }
    while(millis() - startTime < timeout){
     delay(10); 
    }

    if (left){
      HI->pushDriveSpeeds(brakepower, 0);
    }
    else{
      HI->pushDriveSpeeds(0, brakepower);
    }
    delay(100);
     HI->pushDriveSpeeds(0, 0);
}