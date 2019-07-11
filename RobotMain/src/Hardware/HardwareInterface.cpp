#include "Hardware/HardwareInterface.h"

HardwareInterface* HardwareInterface::myInstance = NULL;

void LEncoderInterrupt1(){
    HardwareInterface::i()->LEncoder->update_port_1();
}

void LEncoderInterrupt2(){
    HardwareInterface::i()->LEncoder->update_port_2();
}

void REncoderInterrupt1(){
    HardwareInterface::i()->REncoder->update_port_1();
}

void REncoderInterrupt2(){
    HardwareInterface::i()->REncoder->update_port_2();
}

HardwareInterface::HardwareInterface(){
    HardwareInterface::LMotor = new DriveMotor(LMOTOR_FORWARDS, LMOTOR_BACKWARDS);
    HardwareInterface::RMotor = new DriveMotor(RMOTOR_FORWARDS, RMOTOR_BACKWARDS);
    LMotor->setSpeed(50);
    RMotor->setSpeed(50);
/*
    HardwareInterface::WinchMotor = new DriveMotor(WINCH_UP, WINCH_DOWN);

    HardwareInterface::LEncoder = new Encoder(LENCODER_1, LENCODER_2);
    HardwareInterface::REncoder = new Encoder(RENCODER_1, RENCODER_2);

    pinMode(LENCODER_1,INPUT_PULLUP);
    pinMode(LENCODER_2,INPUT_PULLUP);
    pinMode(RENCODER_1,INPUT_PULLUP);
    pinMode(RENCODER_2,INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(LENCODER_1),LEncoderInterrupt1,RISING);
    attachInterrupt(digitalPinToInterrupt(LENCODER_2),LEncoderInterrupt2,RISING);
    attachInterrupt(digitalPinToInterrupt(RENCODER_1),REncoderInterrupt1,RISING);
    attachInterrupt(digitalPinToInterrupt(RENCODER_2),REncoderInterrupt2,RISING);
*/
    HardwareInterface::qrd0 = new QRD(QRD_IN, 0, 200, 100);
    HardwareInterface::qrd1 = new QRD(QRD_IN, 1, 200, 100);
    HardwareInterface::qrd2 = new QRD(QRD_IN, 2, 200, 100);
    HardwareInterface::qrd3 = new QRD(QRD_IN, 3, 200, 100);
    HardwareInterface::qrdLeft = new QRD(QRD_IN, 4, 200, 100);
    HardwareInterface::qrdRight = new QRD(QRD_IN, 5, 200, 100);

    //HardwareInterface::clawMotor = new ServoMotor(CLAW_SERVO);

    HardwareInterface::QRD_Array[0] = qrd0;
    HardwareInterface::QRD_Array[1] = qrd1;
    HardwareInterface::QRD_Array[2] = qrd2;
    HardwareInterface::QRD_Array[3] = qrd3;
}

bool HardwareInterface::timer(int preset){
  unsigned long currentMillis = millis();

  if(currentMillis - previousMillis >= preset ){
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
        QRD_Vals[i] = QRD_Array[i]->getValue();
        QRD_Maxims[i] = QRD_Array[i]->getMax();
        QRD_Mins[i] = QRD_Array[i]->getMin();
        QRD_Edge[i] = QRD_Array[i]->getEdge();
    }

    //update Motor outputs
    LMotor->update();
    RMotor->update();
    //WinchMotor->update();

    //Encoder values are updated internally via another interrupt

    //Update servo output
    //clawMotor->update();
}