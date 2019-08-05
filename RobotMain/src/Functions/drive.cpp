#include "Functions/drive.h"
#include "Hardware/HardwareInterface.h"
//#include "stateController.h"

void drive_stop_seq(int direction, int timeout, float delta_trip, float kdrift, int maxpower){
    HardwareInterface* HI = HardwareInterface::i();
    
    //must set 
    //delta trip is speed of encoder to trip

    int drift;
    float ave_speed;

    int start_time = millis();
    while(millis() - start_time < 800){
        HI->LMotor->setSpeed( (direction*maxpower) );
        HI->RMotor->setSpeed( (direction*maxpower)/1.13 );
        HI->update();
        ave_speed = (abs(HI->LEncoder->getSpeed()) + abs(HI->REncoder->getSpeed()))/2;
        drift = HI->LEncoder->getCount() - HI->REncoder->getCount();
    }

    while((ave_speed > delta_trip) && ((millis()-start_time) < timeout)){

    ave_speed = (abs(HI->LEncoder->getSpeed()) + abs(HI->REncoder->getSpeed()))/2;
    drift = HI->LEncoder->getCount() - HI->REncoder->getCount();

    Serial.println("ave speed" + String(ave_speed));
    HI->LMotor->setSpeed( (direction*maxpower) - (drift * kdrift));
    HI->RMotor->setSpeed( ((direction*maxpower) + (drift * kdrift))/1.13);
    HI-> update();
    delay(30);
    }
    Serial.println("exit reached!");

    HI->pushDriveSpeeds(-33, -33);
    delay(110);
    HI->pushDriveSpeeds(0, 0);

  }

void stopMoving(){
    HardwareInterface* HI = HardwareInterface::i(); 
    HI->pushDriveSpeeds(-35, -35);
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
    
    HI->pushDriveSpeeds(power_L, power_R);

    Serial.println("Ltics: " + String(Ltics));
    Serial.println("Rtics: " + String(Rtics));
    Serial.println("enError: " + String(encoder_error));
    Serial.println("boost: " + String(boost));
    Serial.println("powL: " + String(power_L));
    Serial.println("powR: " + String(power_R));
    Serial.println("");

    delay(20);
    }
  //brake
  HI->pushDriveSpeeds(-direction*brakepower, -direction*brakepower);

  }

//turn with QRD at end
void QRDTurn_3_L(){
    HardwareInterface* HI = HardwareInterface::i(); 
    int speedL = -40;
    int speedR = 40/1.35;
    Serial.println("QRD turn : left case");
    int start_time = millis();
    while(millis() - start_time < 900){
        HI->pushDriveSpeeds(speedL, speedR);
        }
    HI->update();
        Serial.println("QRD3i: " + String(HI->QRD_Vals[3]));
        Serial.println("QRD4i: " + String(HI->QRD_Vals[4]));
    Serial.println("Forced turn end");
    while(HI->QRD_Vals[3] < 0.8 && HI->QRD_Vals[4] < 0.8){
        
         HI->pushDriveSpeeds(speedL, speedR);
         HI->update();
        Serial.println("QRD3: " + String(HI->QRD_Vals[3]));
        Serial.println("QRD4: " + String(HI->QRD_Vals[4]));
        }
    Serial.println("detected!");
        HI->pushDriveSpeeds(-speedL/2, -speedR/2);
        delay(400);
        HI->pushDriveSpeeds(0, 0);
        
        delay(500);

    }


void turn_single_constant(int target, unsigned int timeout, int robotSpeed){
    HardwareInterface* HI = HardwareInterface::i(); 
    int startTime = millis();
    HI->update();
    int LStartCount = HI->LEncoder->getCount();
    int RStartCount = HI->REncoder->getCount();
    while(millis() - startTime <= timeout){
        if(target >= 0){
            HI->pushDriveSpeeds(0, -robotSpeed/1.35);
            HI->update();
            if(HI->REncoder->getCount() - RStartCount >= target){
                HI->pushDriveSpeeds(0, 0);
                HI->update();
                return;
            }
        }
        else if(target < 0){

            HI->pushDriveSpeeds(-robotSpeed, 0);
            HI->update();
            // Serial.print(" LCount: ");
            // Serial.print(LEncoder->getCount());
            // Serial.print(target);
            // Serial.print(" LStartCount: ");
            // Serial.print(LStartCount);
            if(HI->LEncoder->getCount() - LStartCount <= target){
                HI->pushDriveSpeeds(0, 0);
                HI->update();
                return;
            }
        }
    }
    HI->pushDriveSpeeds(0, 0);
    HI->update();
    }