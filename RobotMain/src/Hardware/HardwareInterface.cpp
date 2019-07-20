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
    Serial.begin(9600);

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
    HardwareInterface::qrd0 = new QRD(QRD_IN, 0, 450, 300, 620);
    HardwareInterface::qrd1 = new QRD(QRD_IN, 1, 140, 120, 170);
    HardwareInterface::qrd2 = new QRD(QRD_IN, 2, 110, 90, 125);
    HardwareInterface::qrd3 = new QRD(QRD_IN, 3, 68, 63, 75);
    HardwareInterface::qrd4 = new QRD(QRD_IN, 4, 79, 75, 83);
    HardwareInterface::qrd5 = new QRD(QRD_IN, 5, 74, 65, 82);
    HardwareInterface::qrd6 = new QRD(QRD_IN, 6, 71, 65, 77);
    HardwareInterface::qrd7 = new QRD(QRD_IN, 7, 245, 150, 363);


    //HardwareInterface::clawMotor = new ServoMotor(CLAW_SERVO);

    HardwareInterface::QRD_Array[0] = qrd0;
    HardwareInterface::QRD_Array[1] = qrd1;
    HardwareInterface::QRD_Array[2] = qrd2;
    HardwareInterface::QRD_Array[3] = qrd3;
    HardwareInterface::QRD_Array[4] = qrd4;
    HardwareInterface::QRD_Array[5] = qrd5;
    HardwareInterface::QRD_Array[6] = qrd6;
    HardwareInterface::QRD_Array[7] = qrd7;
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
        //QRD_Vals[i] = QRD_Array[i]->getValue();
        QRD_Vals[i] = float(QRD_Array[i]->getValue() - QRD_Min[i])/float(QRD_Max[i] - QRD_Min[i]);
    }

    //update Motor outputs
    LMotor->update();
    RMotor->update();
    //WinchMotor->update();

    //Encoder values are updated internally via another interrupt

    //Update servo output
    //clawMotor->update();
}