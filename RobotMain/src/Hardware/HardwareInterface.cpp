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

   HardwareInterface::LEncoder = new Encoder(LENCODER_1, LENCODER_2, LENCODER_DIRECTION, 1);
   HardwareInterface::REncoder = new Encoder(RENCODER_1, RENCODER_2, RENCODER_DIRECTION, 1);

   HardwareInterface::WinchEncoder = new Encoder(WINCH_ENC_1, WINCH_ENC_2, WINCH_ENCODER_DIRECTION, 1);

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

   HardwareInterface::qrd0 = new QRD(QRD_IN, 0, 75, 760);
   HardwareInterface::qrd1 = new QRD(QRD_IN, 1, 60, 481);
   HardwareInterface::qrd2 = new QRD(QRD_IN, 2, 56, 436);
   HardwareInterface::qrd3 = new QRD(QRD_IN, 3, 55, 375);
   HardwareInterface::qrd4 = new QRD(QRD_IN, 4, 56, 470);
   HardwareInterface::qrd5 = new QRD(QRD_IN, 5, 57, 432);
   HardwareInterface::qrd6 = new QRD(QRD_IN, 6, 58, 377);
   HardwareInterface::qrd7 = new QRD(QRD_IN, 7, 69, 619);

// // //   //  For calibrating
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
        
    }
    
    //update Motor outputs
    LMotor->update();
    RMotor->update();
    //WinchMotor->update();


    LEncoder->update();
    REncoder->update();
    
    //Update servo output
    //clawMotor->update();
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

void HardwareInterface::pushDriveSpeeds(int Lspeed, int Rspeed){
    int Lfinal = motorcap(Lspeed, 100, -100);
    int Rfinal = motorcap(Rspeed, 100, -100);
    LMotor->setSpeed(Lfinal);
    RMotor->setSpeed(Rfinal);
    LMotor->update();
    RMotor->update();
}

void HardwareInterface::pushWinchSpeed(int speed){
    int final = motorcap(speed, 100, -100);
    WinchMotor->setSpeed(final);
    WinchMotor->update();
}
