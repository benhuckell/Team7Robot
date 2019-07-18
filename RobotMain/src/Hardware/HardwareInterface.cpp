#include "Hardware/HardwareInterface.h"

HardwareInterface* HardwareInterface::myInstance = NULL;


void LEncoderInterrupt(){
    //Serial.print(" InterruptingL... ");
    HardwareInterface::i()->LEncoder->ISR();
}

void REncoderInterrupt(){
    //Serial.print(" InterruptingR... ");
    HardwareInterface::i()->REncoder->ISR();
}

HardwareInterface::HardwareInterface(){
    Serial.begin(9600);

    HardwareInterface::LMotor = new DriveMotor(LMOTOR_FORWARDS, LMOTOR_BACKWARDS);
    HardwareInterface::RMotor = new DriveMotor(RMOTOR_FORWARDS, RMOTOR_BACKWARDS);
    LMotor->setSpeed(50);
    RMotor->setSpeed(50);
/*
    HardwareInterface::WinchMotor = new DriveMotor(WINCH_UP, WINCH_DOWN);
*/
    HardwareInterface::LEncoder = new Encoder(LENCODER_1, LENCODER_2);
    HardwareInterface::REncoder = new Encoder(RENCODER_1, RENCODER_2);

    pinMode(LENCODER_1,INPUT_PULLUP);
    pinMode(LENCODER_2,INPUT_PULLUP);
    pinMode(RENCODER_1,INPUT_PULLUP);
    pinMode(RENCODER_2,INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(LENCODER_1),LEncoderInterrupt,RISING);
    attachInterrupt(digitalPinToInterrupt(RENCODER_1),REncoderInterrupt,RISING);

    HardwareInterface::qrd0 = new QRD(QRD_IN, 0, 100, 60, 363);
    HardwareInterface::qrd1 = new QRD(QRD_IN, 1, 100, 63, 371);
    HardwareInterface::qrd2 = new QRD(QRD_IN, 2, 100, 62, 398);
    HardwareInterface::qrd3 = new QRD(QRD_IN, 3, 100, 62, 313);
    HardwareInterface::qrdLeft = new QRD(QRD_IN, 4, 100, 50, 200);
    HardwareInterface::qrdRight = new QRD(QRD_IN, 5, 100, 50, 200);

    //HardwareInterface::clawMotor = new ServoMotor(CLAW_SERVO);

    HardwareInterface::QRD_Array[0] = qrd0;
    HardwareInterface::QRD_Array[1] = qrd1;
    HardwareInterface::QRD_Array[2] = qrd2;
    HardwareInterface::QRD_Array[3] = qrd3;
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
        QRD_Thresh[i] = QRD_Array[i]->getThresh();
        QRD_Max[i] = QRD_Array[i]->getMax();
        QRD_Min[i] = QRD_Array[i]->getMin();
        QRD_Vals[i] = float(QRD_Array[i]->getValue() - QRD_Min[i])/float(QRD_Max[i] - QRD_Min[i]);
    }

    //update Motor outputs
    LMotor->update();
    RMotor->update();
    //WinchMotor->update();

    //Encoder values are updated internally via another interrupt
    lastLSpeed = LEncoder->getSpeed();
    lastRSpeed = REncoder->getSpeed();
    
    //Update servo output
    //clawMotor->update();
}

bool HardwareInterface::robotWasBumped(){
    if(lastLSpeed+lastRSpeed - (LEncoder->getSpeed()+REncoder->getSpeed()) > bumpThresholdVal){
        return true;
    }
    return false;
}

bool HardwareInterface::robotHitPost(){
    if(LEncoder->getSpeed()+REncoder->getSpeed() < postThresholdVal){
        return true;
    } 
    return false;
}