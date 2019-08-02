#include "Hardware/HardwareInterface.h"

HardwareInterface* HardwareInterface::myInstance = NULL;


void LEncoderInterrupt(){
   HardwareInterface::i()->LEncoder->ISR();
}

void REncoderInterrupt(){
   HardwareInterface::i()->REncoder->ISR();
}

void WinchEncoderInterrupt(){
    HardwareInterface::i()->WinchEncoder->ISR();
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
   //attachInterrupt(digitalPinToInterrupt(WINCH_ENC_1),REncoderInterrupt,RISING);

   HardwareInterface::qrd0 = new QRD(QRD_IN, 0, 61, 674);
   HardwareInterface::qrd1 = new QRD(QRD_IN, 1, 54, 311);
   HardwareInterface::qrd2 = new QRD(QRD_IN, 2, 52, 245);
   HardwareInterface::qrd3 = new QRD(QRD_IN, 3, 51, 152);
   HardwareInterface::qrd4 = new QRD(QRD_IN, 4, 52, 200);
   HardwareInterface::qrd5 = new QRD(QRD_IN, 5, 52, 239);
   HardwareInterface::qrd6 = new QRD(QRD_IN, 6, 53, 161);
   HardwareInterface::qrd7 = new QRD(QRD_IN, 7, 60, 484);

//   //  For calibrating
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
       QRD_Array[i]->update();
       //QRD_Vals[i] = QRD_Array[i]->getValue();
       //QRD_Thresh[i] = QRD_Array[i]->getThresh();
       QRD_RAW[i] = QRD_Array[i]->getValue();
       QRD_Vals[i] = float(QRD_Array[i]->getValue() - QRD_Min[i])/float(QRD_Max[i] - QRD_Min[i]);
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
    if(WinchSpeed>100){
        WinchSpeed=100;
    }
    if(WinchSpeed<-100){
        WinchSpeed=-100;
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

void HardwareInterface::QRDTurn(bool turnRight){
    if(turnRight){
        while(errorHistory.back() - errorHistory.front() < 10.0){
            LMotor->setSpeed(30);
            RMotor->setSpeed(-30);
            update();
            errorHistory.push(getWeightedError());
            errorHistory.pop();
        }
    }
    else{//turn left
        while(errorHistory.back() - errorHistory.front() < -10.0){
            LMotor->setSpeed(-30);
            RMotor->setSpeed(30);
            update();
            errorHistory.push(getWeightedError());
            errorHistory.pop();
        }
    }
}