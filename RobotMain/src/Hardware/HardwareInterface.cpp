#include "Hardware/HardwareInterface.h"


HardwareInterface* HardwareInterface::myInstance = NULL;

HardwareInterface::HardwareInterface(){
    HardwareInterface::LMotor1 = new DriveMotor(LMOTOR_FORWARDS_1, LMOTOR_BACKWARDS_1);
    HardwareInterface::LMotor2 = new DriveMotor(LMOTOR_FORWARDS_2, LMOTOR_BACKWARDS_2);
    HardwareInterface::RMotor1 = new DriveMotor(RMOTOR_FORWARDS_1, RMOTOR_BACKWARDS_1);
    HardwareInterface::RMotor2 = new DriveMotor(RMOTOR_FORWARDS_2, RMOTOR_BACKWARDS_2);

    HardwareInterface::WinchMotor = new DriveMotor(WINCH_UP, WINCH_DOWN);

    HardwareInterface::LEncoder = new Encoder(LENCODER_1, LENCODER_2);
    HardwareInterface::REncoder = new Encoder(RENCODER_1, RENCODER_2);

    HardwareInterface::qrd0 = new QRD(QRD_IN, 0, 200, 50);
    HardwareInterface::qrd1 = new QRD(QRD_IN, 1, 200, 50);
    HardwareInterface::qrd2 = new QRD(QRD_IN, 2, 200, 50);
    HardwareInterface::qrd3 = new QRD(QRD_IN, 3, 200, 50);
    HardwareInterface::qrdLeft = new QRD(QRD_IN, 4, 200, 100);
    HardwareInterface::qrdRight = new QRD(QRD_IN, 5, 200, 100);

    HardwareInterface::clawMotor = new ServoMotor(CLAW_SERVO);

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
    //update QRD values
    display.clearDisplay();
    display.setCursor(0, 0);
    for(int i = 0; i < NUM_QRD_SENSORS; i++){
        QRD_Array[i]->update();
        QRD_Vals[i] = QRD_Array[i]->getValue();
        QRD_Maxims[i] = QRD_Array[i]->getMax();
        QRD_Mins[i] = QRD_Array[i]->getMin();
        QRD_Edge[i] = QRD_Array[i]->getEdge();
        display.print(QRD_Vals[i]);
        display.print(" ");
    }
    
    display.display();
    //update Motor outputs
    LMotor1->update();
    LMotor2->update();
    RMotor1->update();
    RMotor2->update();
    WinchMotor->update();

    //Encoder values are updated internally via another interrupt

    //Update servo output
    clawMotor->update();
    return;
}