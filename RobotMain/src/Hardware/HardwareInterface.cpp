#include "Hardware/HardwareInterface.h"


HardwareInterface* HardwareInterface::myInstance = NULL;

HardwareInterface::HardwareInterface(){
    HardwareInterface::RMotor = new DriveMotor(RMOTOR_FORWARDS, RMOTOR_BACKWARDS);
    HardwareInterface::LMotor = new DriveMotor(LMOTOR_FORWARDS, LMOTOR_BACKWARDS);

    //HardwareInterface::WinchMotor = new DriveMotor(WINCH_UP, WINCH_DOWN);

    //HardwareInterface::LEncoder = new Encoder(LENCODER_1, LENCODER_2);
    //HardwareInterface::REncoder = new Encoder(RENCODER_1, RENCODER_2);

    HardwareInterface::qrd0 = new QRD(QRD_IN, 0, 200, 50);
    HardwareInterface::qrd1 = new QRD(QRD_IN, 1, 200, 50);
    HardwareInterface::qrd2 = new QRD(QRD_IN, 2, 200, 50);
    HardwareInterface::qrd3 = new QRD(QRD_IN, 3, 200, 50);
    HardwareInterface::qrdLeft = new QRD(QRD_IN, 4, 200, 100);
    HardwareInterface::qrdRight = new QRD(QRD_IN, 5, 200, 100);

    //HardwareInterface::clawMotor = new ServoMotor(CLAW_SERVO);

    HardwareInterface::QRD_Array[0] = qrd0;
    HardwareInterface::QRD_Array[1] = qrd1;
    HardwareInterface::QRD_Array[2] = qrd2;
    HardwareInterface::QRD_Array[3] = qrd3;
}

HardwareInterface* HardwareInterface::i(){
    if(myInstance == NULL){
        myInstance = new HardwareInterface();
        return myInstance;
    }
    return myInstance;
}

void HardwareInterface::update(){
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
    clawMotor->update();
    return;
}