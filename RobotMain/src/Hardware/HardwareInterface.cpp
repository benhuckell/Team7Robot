#include "Hardware/HardwareInterface.h"

HardwareInterface* HardwareInterface::myInstance = NULL;


void LEncoderInterrupt(){
   HardwareInterface::i()->LEncoder->ISR();
}

void REncoderInterrupt(){
   HardwareInterface::i()->REncoder->ISR();
}

void WinchEncoderInterrupt(){
    HardwareInterface::i()->WinchEncoder->ISR_winch();
}

HardwareInterface::HardwareInterface(){
   Serial.begin(115200);

   HardwareInterface::LMotor = new DriveMotor(LMOTOR_FORWARDS, LMOTOR_BACKWARDS);
   HardwareInterface::RMotor = new DriveMotor(RMOTOR_FORWARDS, RMOTOR_BACKWARDS);
   LMotor->setSpeed(50);
   RMotor->setSpeed(50);

   HardwareInterface::WinchMotor = new DriveMotor(WINCH_UP, WINCH_DOWN);

   HardwareInterface::LEncoder = new Encoder(LENCODER_1, LENCODER_2, LENCODER_DIRECTION);
   HardwareInterface::REncoder = new Encoder(RENCODER_1, RENCODER_2, RENCODER_DIRECTION);

   HardwareInterface::WinchEncoder = new Encoder(WINCH_ENC_1, WINCH_ENC_2, WINCH_ENCODER_DIRECTION);

   pinMode(LENCODER_1,INPUT_PULLUP);
   pinMode(LENCODER_2,INPUT_PULLUP);
   pinMode(RENCODER_1,INPUT_PULLUP);
   pinMode(RENCODER_2,INPUT_PULLUP);

   pinMode(WINCH_ENC_1,INPUT_PULLUP);
   pinMode(WINCH_ENC_2,INPUT_PULLUP);
   pinMode(LED_RED, OUTPUT);
   pinMode(LED_BLUE, OUTPUT);
   attachInterrupt(digitalPinToInterrupt(LENCODER_1),LEncoderInterrupt,RISING);
   attachInterrupt(digitalPinToInterrupt(RENCODER_1),REncoderInterrupt,RISING);
   attachInterrupt(digitalPinToInterrupt(WINCH_ENC_1),WinchEncoderInterrupt,RISING);

   HardwareInterface::qrd0 = new QRD(QRD_IN, 0, 70, 777);
   HardwareInterface::qrd1 = new QRD(QRD_IN, 1, 59, 506);
   HardwareInterface::qrd2 = new QRD(QRD_IN, 2, 55, 460);
   HardwareInterface::qrd3 = new QRD(QRD_IN, 3, 54, 389);
   HardwareInterface::qrd4 = new QRD(QRD_IN, 4, 55, 474);
   HardwareInterface::qrd5 = new QRD(QRD_IN, 5, 56, 444);
   HardwareInterface::qrd6 = new QRD(QRD_IN, 6, 58, 394);
   HardwareInterface::qrd7 = new QRD(QRD_IN, 7, 70, 651);

// //   //  For calibrating
//    HardwareInterface::qrd0 = new QRD(QRD_IN, 0, 0, 1000);
//    HardwareInterface::qrd1 = new QRD(QRD_IN, 1, 0, 1000);
//    HardwareInterface::qrd2 = new QRD(QRD_IN, 2, 0, 1000);
//    HardwareInterface::qrd3 = new QRD(QRD_IN, 3, 0, 1000);
//    HardwareInterface::qrd4 = new QRD(QRD_IN, 4, 0, 1000);
//    HardwareInterface::qrd5 = new QRD(QRD_IN, 5, 0, 1000);
//    HardwareInterface::qrd6 = new QRD(QRD_IN, 6, 0, 1000);
//    HardwareInterface::qrd7 = new QRD(QRD_IN, 7, 0, 1000);

   HardwareInterface::clawMotor = new ServoMotor(CLAW_SERVO);

   HardwareInterface::QRD_Array[0] = qrd0;
   HardwareInterface::QRD_Array[1] = qrd1;
   HardwareInterface::QRD_Array[2] = qrd2;
   HardwareInterface::QRD_Array[3] = qrd3;
   HardwareInterface::QRD_Array[4] = qrd4;
   HardwareInterface::QRD_Array[5] = qrd5;
   HardwareInterface::QRD_Array[6] = qrd6;
   HardwareInterface::QRD_Array[7] = qrd7;

  
   for(int i = 0; i < NUM_QRD_SENSORS; i++){
       QRD_Max[i] = QRD_Array[i]->getMax();
       QRD_Min[i] = QRD_Array[i]->getMin();
       QRD_Vals[i] = float(QRD_Array[i]->getValue() - QRD_Min[i])/float(QRD_Max[i] - QRD_Min[i]);
       //QRD_Prev[i] = QRD_Vals[i];
       QRD_RAW[i] = QRD_Array[i]->getValue();
   }
}

bool HardwareInterface::timer(unsigned int preset){
 unsigned long currentMillis = millis();

 if(currentMillis - previousMillis >= preset){
   previousMillis = currentMillis;
   return true;
 }
 else{
   return false;
 }
}
HardwareInterface* HardwareInterface::i(){
   if(myInstance == NULL){
       myInstance = new HardwareInterface();
       return myInstance;
   }
   return myInstance;
}

void HardwareInterface::update(){
   //update QRD values
   for(int i = 0; i < NUM_QRD_SENSORS; i++){
       //QRD_Prev[i] = QRD_Vals[i];//get last normalized value

       QRD_Array[i]->update();//then update vals
       
       QRD_RAW[i] = QRD_Array[i]->getValue();
       QRD_Vals[i] = float(QRD_Array[i]->getValue() - QRD_Min[i])/float(QRD_Max[i] - QRD_Min[i]);
       
    //    if(QRD_Prev[i] == 0 && QRD_Vals[i] == 1.0){//filter out spikes in QRD_values
    //        QRD_Vals[i] = 0;
    //    }
   }
   //update Motor outputs
   LMotor->update();
   RMotor->update();
   //WinchMotor->update();

   //Encoder values are updated internally via another interrupt
  
   lastRSpeed = REncoder->getSpeed();
   lastLSpeed = LEncoder->getSpeed();

   LEncoder->update();
   REncoder->update();
  
   //Update servo output
   //clawMotor->update();
}



bool HardwareInterface::robotWasBumped(){
   // Serial.print("lastLSpeed: ");
   // Serial.print(lastLSpeed);
   Serial.print("lastRSpeed: ");
   Serial.print(lastRSpeed);
   // Serial.print("LSpeed: ");
   // Serial.print(LEncoder->getSpeed());
   Serial.print(" RSpeed: ");
   Serial.println(REncoder->getSpeed());
   if(lastLSpeed+lastRSpeed - (LEncoder->getSpeed()+REncoder->getSpeed()) > bumpThresholdVal){
       //Serial.print(" bumped ");
       return true;
   }
   //Serial.print(" no bump ");
   return false;
}

bool HardwareInterface::robotHitPost(){
   if(LEncoder->getSpeed()+REncoder->getSpeed() < postThresholdVal){
       return true;
   }
   return false;
}

void HardwareInterface::turn_time(int target, int timeout, float kdrift, float k_p){
    //neg target is to left
    //pos target is to right
   // Serial.print("started turn_time \n");
   int L_tics_start = LEncoder->getCount();
   int R_tics_start = REncoder->getCount();

   int start_time = millis();
   int net_time = 0;
   //Serial.print("loop entered \n");
   while(net_time < timeout){
       update();
       errorHistory.pop();
       errorHistory.push(getWeightedError());
       net_time = millis() - start_time;
       int L_tics = LEncoder->getCount() - L_tics_start;
       int R_tics = REncoder->getCount() - R_tics_start;
       //Serial.println("Ltic: " + String(L_tics) + "| Rtics : " + String(R_tics));
       int drift_error = L_tics + R_tics; //positive L and R is forwards
       //when negitive, robot is turning too much to left
       //Serial.println("Drift error: " + String(drift_error));
       float pos = (L_tics - R_tics) /  2;
       float error = pos - target;
       //Serial.println("pos: " + String(pos) + "error: " + String(error));
       float motor_power_1 =  k_p * error;

       //motor cap: can be generally inplemente
       if (motor_power_1 > 60){
           motor_power_1 = 60;
       }
       else if (motor_power_1 < -60 ){
               motor_power_1 = -60;
       }
       float motor_power_2_L = (-motor_power_1) + (kdrift* float(drift_error));
       float motor_power_2_R = (motor_power_1) + (kdrift* float(drift_error));
       //Serial.println("driftLterm: " + String(kdrift*drift_error));

       //output motor power
      
       //Serial.println("speedL: " + String(motor_power_2_L) + " | speedR " + String(motor_power_2_R));
       //Serial.println(" ");
       LMotor->setSpeed(motor_power_2_L);
       RMotor->setSpeed(motor_power_2_R/1.05);

       LMotor->update();
       RMotor->update();


   }
    //Serial.print("loop exit \n");

   //brake power

    
   LMotor->setSpeed(0);
   RMotor->setSpeed(0);
   LMotor->update();
   RMotor->update();
    delay(100);

}


void HardwareInterface::turn_single_backwards(int target, int timeout, float kdrift, float k_p){
    //neg target is to left
    //pos target is to right
   // Serial.print("started turn_time \n");
   int L_tics_start = LEncoder->getCount();
   int R_tics_start = REncoder->getCount();

   int start_time = millis();
   int net_time = 0;
   //Serial.print("loop entered \n");
   while(net_time < timeout){
       update();
       errorHistory.pop();
       errorHistory.push(getWeightedError());

       net_time = millis() - start_time;
       int L_tics = LEncoder->getCount() - L_tics_start;
       int R_tics = REncoder->getCount() - R_tics_start;
       Serial.println("Rtic: " + String(R_tics) + "| Ltics : " + String(L_tics));
      
       //when negitive, robot is turning too much to left
       //Serial.println("Drift error: " + String(drift_error));
       int errorL = 0;
       int errorR = 0;
       int tarL = 0;
       int tarR = 0;
       float motor_power_L = 0.0;
       float motor_power_R = 0.0;
       if(target > 0){
           //right turn, R motor moves backwards
           tarL = 0;
           tarR = -target;//negative so wheel moves backwards
           errorL = tarL - L_tics;
           errorR = tarR - R_tics; 
           motor_power_L =  k_p * errorL;
           motor_power_R =  k_p * errorR;

       }
       else{
           //left turn, L motor moves backwards
           tarR = 0;
           tarL = target;
           errorR = tarR - R_tics;
           errorL = tarL - L_tics; //postive going to zero
           motor_power_L =  k_p * errorL;
           motor_power_R =  k_p * errorR;
       }
        //Serial.println("Drift error: " + String(drift_error));

       //output motor power
      
       Serial.println("speedR: " + String(motor_power_R) + " | speedL " + String(motor_power_L));
       Serial.println("tarR: " + String(tarR) + " | tarL: " + String(tarL));
       Serial.println("errorR: " + String(errorR) + " | tarL: " + String(errorL));
       Serial.println(" ");
       LMotor->setSpeed(motor_power_L);
       RMotor->setSpeed(motor_power_R);

       LMotor->update();
       RMotor->update();


   }
    //Serial.print("loop exit \n");

   //brake power

   LMotor->setSpeed(0);
   RMotor->setSpeed(0);
   LMotor->update();
   RMotor->update();
   delay(100);

}



void HardwareInterface::turn_single(int target, int motor, int dir, int timeout, float k_p){

    //motor = 1 is left motor
    //motor = -1 is right motor

    //dir = 1 is forward
    //dir = -1 is backwards

    //f

   int L_tics_start = LEncoder->getCount();
   int R_tics_start = REncoder->getCount();

   int start_time = millis();
   int net_time = 0;
   //Serial.print("loop entered \n");
   while(net_time < timeout){
       update();
       errorHistory.pop();
       errorHistory.push(getWeightedError());
       net_time = millis() - start_time;
       int L_tics = LEncoder->getCount() - L_tics_start;
       int R_tics = REncoder->getCount() - R_tics_start;
       Serial.println("Rtic: " + String(R_tics) + "| Ltics : " + String(L_tics));
      
       //when negitive, robot is turning too much to left
       //Serial.println("Drift error: " + String(drift_error));
       int errorL = 0;
       int errorR = 0;
       float motor_power_L = 0.0;
       float motor_power_R = 0.0;
       
       if(motor == 1){
           //left motor
            errorL = (dir*target) - L_tics;
            errorR = -20*dir;
       }
       else{
           //rght motor
           errorL = -dir*20;
           errorR = (dir*target) - R_tics;
       }
        //Serial.println("Drift error: " + String(drift_error));
        motor_power_L = errorL*k_p;
        motor_power_R = errorR*k_p;

        //cap motor power
        if(motor_power_L > 80){
            motor_power_L = 80;
        }

        if(motor_power_R > 80){
            motor_power_R = 80;
        }
       //output motor power
      
       Serial.println("speedR: " + String(motor_power_R) + " | speedL " + String(motor_power_L));
       Serial.println("errorR: " + String(errorR) + " | error: " + String(errorL));
       Serial.println(" ");
       LMotor->setSpeed(motor_power_L);
       RMotor->setSpeed(motor_power_R);

       LMotor->update();
       RMotor->update();


   }
    //Serial.print("loop exit \n");

   //brake power

   LMotor->setSpeed(0);
   RMotor->setSpeed(0);
   LMotor->update();
   RMotor->update();
   delay(100);

}

void HardwareInterface::moveIntake() {
    //set the height that the winch raises the entire assembly to
    //need to use PID to get to the correct height
    int tick_num=WinchEncoder->getCount();
    int tickError=winchTickTarget-tick_num;

    float WinchSpeed=0;
    WinchSpeed = Winch_P_gain*tickError;

    //to set what happens at edge cases
    if(WinchSpeed>60){
        WinchSpeed=60;
    }
    if(WinchSpeed<-60){
        WinchSpeed=-60;
    }

    WinchMotor->setSpeed(WinchSpeed);
    WinchMotor->update();
}

void HardwareInterface::checkForRock(){
    if(!(clawCurrentAngle==clawFullyOpen)){
        clawMotor->clawSetPos(clawFullyOpen);
        clawCurrentAngle=clawFullyOpen;
        delay(1000);
    }

    clawMotor->clawSetPos(clawWithRock);
    delay(2000);

    if(digitalRead(LIM_SWITCH_PIN==LOW)){
        hasRock=false;
    } else{
        hasRock=true;
    }
    clawCurrentAngle=clawWithRock;
    
        
}

float HardwareInterface::getWeightedError(){
    float sum = 0;
    float weightedSum = 0;
    bool onBlack = false;
    for(int i = 0; i < NUM_QRD_SENSORS; i++){
        sum += QRD_Vals[i];
        weightedSum += QRD_Vals[i]*positionVector[i];
    }

    if(sum > 0.45){
        onBlack = true;
    }

    if(onBlack){
        return weightedSum;
    } else if(errorHistory.back() < 0){
        return positionVector[0];
    } else {
        return positionVector[NUM_QRD_SENSORS-1];
    }
}

void HardwareInterface::turn_single_constant(int target, unsigned int timeout, int robotSpeed){
    int startTime = millis();
    update();
    int LStartCount = LEncoder->getCount();
    int RStartCount = REncoder->getCount();
    while(millis() - startTime <= timeout){
        if(target >= 0){
            LMotor->setSpeed(0);
            RMotor->setSpeed(-robotSpeed/1.35);
            update();
            if(REncoder->getCount() - RStartCount <= -target){
                LMotor->setSpeed(0);
                RMotor->setSpeed(0);
                update();
                return;
            }
        }
        else if(target < 0){
            LMotor->setSpeed(-robotSpeed);
            RMotor->setSpeed(0);
            update();
            // Serial.print(" LCount: ");
            // Serial.print(LEncoder->getCount());
            // Serial.print(target);
            // Serial.print(" LStartCount: ");
            // Serial.print(LStartCount);
            if(LEncoder->getCount() - LStartCount <= target){
                LMotor->setSpeed(0);
                RMotor->setSpeed(0);
                update();
                return;
            }
        }
    }
    LMotor->setSpeed(0);
    RMotor->setSpeed(0);
    update();
}

void HardwareInterface::moveIntake_const_speed(){
    //Serial.println("Encoder: Winch: " + String(WinchEncoder->getCount()));
    WinchSpeed = 40;

    WinchMotor->setSpeed(WinchSpeed);
    //Serial.println("speed:" + String(WinchSpeed));
    WinchMotor->update();
    WinchEncoder->update();
}

void HardwareInterface::QRDTurn_2(bool turnRight){
    Serial.println("t");
    //twice for full reset
    update();
    errorHistory.push(getWeightedError());
    errorHistory.pop();
    update();
    errorHistory.push(getWeightedError());
    errorHistory.pop();
    if(turnRight){
        Serial.println("QRD turn : right case");
        while(errorHistory.back() - errorHistory.front() < 10.0){
            LMotor->setSpeed(30);
            RMotor->setSpeed(-30);
            update();
            errorHistory.push(getWeightedError());
            errorHistory.pop();
        }
    }
    else{//turn left
        Serial.println("QRD turn : left case");
        Serial.println("error net: " + String(errorHistory.back() - errorHistory.front()));

        while(errorHistory.back() - errorHistory.front() < 10.0){
            digitalWrite(LED_BLUE, HIGH);
            LMotor->setSpeed(-30);
            RMotor->setSpeed(30);
            update();
            errorHistory.push(getWeightedError());
            errorHistory.pop();
            Serial.println("error net2: " + String(errorHistory.back() - errorHistory.front()));
            Serial.println("QRD output: " + String(QRD_Vals[0]) + " " + String(QRD_Vals[1]) + " " + String(QRD_Vals[2]) + " " + String(QRD_Vals[3]) + " " + String(QRD_Vals[4]) + " " + String(QRD_Vals[5]) + " " + String(QRD_Vals[6]) + " " + String(QRD_Vals[7]) + " ");
            Serial.println("Error back: " + String(errorHistory.back()));
            Serial.println("Error front: " + String(errorHistory.front()));
            Serial.println("");
        }
        digitalWrite(LED_BLUE,LOW);
        LMotor->setSpeed(0);
        RMotor->setSpeed(0);
        delay(1000);
    }
}

//turn with QRD at end
void HardwareInterface::QRDTurn_3_L(){
    int speedL = -40;
    int speedR = 40/1.35;
    Serial.println("QRD turn : left case");
    int start_time = millis();
    while(millis() - start_time < 900){
        RMotor->setSpeed(speedR);
        LMotor->setSpeed(speedL);
        RMotor->update();
        LMotor->update();
    }
    update();
        Serial.println("QRD3i: " + String(QRD_Vals[3]));
        Serial.println("QRD4i: " + String(QRD_Vals[4]));
    Serial.println("Forced turn end");
    while(QRD_Vals[3] < 0.8 && QRD_Vals[4] < 0.8){
        RMotor->setSpeed(speedR);
        LMotor->setSpeed(speedL);
        //RMotor->update();
        //LMotor->update();
        update();
        Serial.println("QRD3: " + String(QRD_Vals[3]));
        Serial.println("QRD4: " + String(QRD_Vals[4]));
        }
    Serial.println("detected!");
        RMotor->setSpeed(-speedR/2);
        LMotor->setSpeed(-speedL/2);
        RMotor->update();
        LMotor->update();
        delay(400);
        RMotor->setSpeed(0);
        LMotor->setSpeed(0);
        RMotor->update();
        LMotor->update();
        delay(500);

    }

//turn with QRD at end
void HardwareInterface::QRDTurnNew(bool rightTurn){
    int speedL;
    int speedR;
    
    if(rightTurn){
        speedL = 35;
        speedR = -35/1.35;
    }
    else{
        speedL = -35;
        speedR = 35/1.35;
    }

    Serial.println("QRD turn : left case");
    int start_time = millis();
    while(millis() - start_time < 900){
        RMotor->setSpeed(speedR);
        LMotor->setSpeed(speedL);
        RMotor->update();
        LMotor->update();
    }
    
    update();
    Serial.println("QRD3i: " + String(QRD_Vals[3]));
    Serial.println("QRD4i: " + String(QRD_Vals[4]));
    Serial.println("Forced turn end");

    while(QRD_Vals[3] < 0.8 && QRD_Vals[4] < 0.8){
        RMotor->setSpeed(speedR);
        LMotor->setSpeed(speedL);
        //RMotor->update();
        //LMotor->update();
        update();
        Serial.println("QRD3: " + String(QRD_Vals[3]));
        Serial.println("QRD4: " + String(QRD_Vals[4]));
        }
    Serial.println("detected!");
    // HI->RMotor->setSpeed(-speedR/2);
    // HI->LMotor->setSpeed(-speedL/2);
    // HI->RMotor->update();
    // HI->LMotor->update();
    // delay(400);
    RMotor->setSpeed(0);
    LMotor->setSpeed(0);
    RMotor->update();
    LMotor->update();
    delay(500);

    }

    



void HardwareInterface::jdubDrive(int direction, int target, int maxpower, int minpower, unsigned int timeout, float kaccel, float kdeaccel, float kdrift){

  int netpower = maxpower - minpower;
  float boost;
  int brakepower = 15;

  int Ltics_start = LEncoder->getCount();
  int Rtics_start = REncoder->getCount();

  int encoderaverage = 0;
  int start_time = millis();
  unsigned int net_time = 0;

  while( (encoderaverage < target) && (net_time < timeout)){

    net_time  = millis() - start_time;

    int Ltics = abs(LEncoder->getCount() - Ltics_start); 
    int Rtics = abs(REncoder->getCount() - Rtics_start); 
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

    int power_L = direction*motorcap(minpower + boost*netpower, 100, 0) - (direction*encoder_error*kdrift);
    int power_R = direction*motorcap(minpower + boost*netpower, 100, 0) + (direction*encoder_error*kdrift);
    
    LMotor->setSpeed(power_L);
    RMotor->setSpeed(power_R);
    LMotor->update();
    RMotor->update();

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
    LMotor->setSpeed(-direction*brakepower);
    RMotor->setSpeed(-direction*brakepower);
    LMotor->update();
    RMotor->update();
  }

int HardwareInterface::motorcap(int input, int maxpower, int minpower){

    if(input > maxpower){
        return maxpower;
    }
    else if(input < minpower){
        return minpower;
    }
    else{
        return input;
    }
}

void HardwareInterface::getStone_const_speed(int tickTarget){
        HI->WinchEncoder->winch_dir=-1;

        int startingTicks = HI->WinchEncoder->getCount();

        //raise intake1
        while((HI->WinchEncoder->getCount() - startingTicks) < tickTarget){
            HI->moveIntake_const_speed();
            //Serial.println("en: " + String(HI->WinchEncoder->getCount()));
            //Serial.println("winch dir: " + String(HI->WinchEncoder->winch_dir));
        }

        Serial.println("Done first up");

        HI->WinchMotor->setSpeed(0);
        HI->WinchMotor->update();

        //closing the claw around the rock

        int startClawTime = millis();
        while(millis()- startClawTime < 1400){
            HI->clawMotor->clawSetPos(200);
        }

        Serial.println("Closed claw");

        delay(1000);

        HI->WinchEncoder->winch_dir=-1;


        //lifting up to make sure rock isn't still in the pole mount
        while((HI->WinchEncoder->getCount() - startingTicks) < tickTarget + 50){
            HI-> moveIntake_const_speed();
            Serial.println("en: " + String(HI->WinchEncoder->getCount()));
            Serial.println("winch dir: " + String(HI->WinchEncoder->winch_dir));
        }

        Serial.println("Done");

        HI->WinchMotor->setSpeed(0);
        HI->WinchMotor->update();
    }