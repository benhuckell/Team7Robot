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

    HardwareInterface::WinchMotor = new DriveMotor(WINCH_UP, WINCH_DOWN);

    HardwareInterface::LEncoder = new Encoder(LENCODER_1, LENCODER_2);
    HardwareInterface::REncoder = new Encoder(RENCODER_1, RENCODER_2);

    pinMode(LENCODER_1,INPUT_PULLUP);
    pinMode(LENCODER_2,INPUT_PULLUP);
    pinMode(RENCODER_1,INPUT_PULLUP);
    pinMode(RENCODER_2,INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(LENCODER_1),LEncoderInterrupt,RISING);
    attachInterrupt(digitalPinToInterrupt(RENCODER_1),REncoderInterrupt,RISING);

    HardwareInterface::qrd0 = new QRD(QRD_IN, 0, 100, 75, 470);
    HardwareInterface::qrd1 = new QRD(QRD_IN, 1, 90, 62, 91);
    HardwareInterface::qrd2 = new QRD(QRD_IN, 2, 65, 61, 105);
    HardwareInterface::qrd3 = new QRD(QRD_IN, 3, 65, 60, 80);
    HardwareInterface::qrd4 = new QRD(QRD_IN, 4, 65, 61, 102);
    HardwareInterface::qrd5 = new QRD(QRD_IN, 5, 70, 61, 80);
    HardwareInterface::qrd6 = new QRD(QRD_IN, 6, 70, 62, 88);
    HardwareInterface::qrd7 = new QRD(QRD_IN, 7, 100, 67, 234);

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