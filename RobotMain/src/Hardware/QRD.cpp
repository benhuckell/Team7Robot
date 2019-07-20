#include "Hardware/QRD.h"

QRD::QRD(PinName QRD_port, int QRD_Index, int QRD_Thresh, int QRD_Min, int QRD_Max){
    QRD::QRD_port = QRD_port;
    QRD::QRD_Index = QRD_Index;
    QRD::QRD_Thresh = QRD_Thresh;
    QRD::QRD_Min = QRD_Min;
    QRD::QRD_Max = QRD_Max;

    pinMode(Select_0, OUTPUT);
    pinMode(Select_1, OUTPUT);
    pinMode(Select_2, OUTPUT);
}

void QRD::update() {
    //update values
    digitalWrite(Select_0,states[QRD_Index][0]);
    digitalWrite(Select_1,states[QRD_Index][1]);
    digitalWrite(Select_2,states[QRD_Index][2]);
    //Serial.print(value);
    value = analogRead(QRD_port); //read from first multiplexer
    if(value < QRD_Min) { value = QRD_Min; }
    if(value > QRD_Max) { value = QRD_Max; }
}

int QRD::getValue() {
    return value;
}

int QRD::getThresh() {
    return QRD_Thresh;
}

int QRD::getMin() {
    return QRD_Min;
}

int QRD::getMax(){
    return QRD_Max;
}