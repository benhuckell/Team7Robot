#include "Hardware/QRD.h"

QRD::QRD(PinName QRD_port, int QRD_Index, int QRD_Max, int QRD_Min){
    QRD::QRD_port = QRD_port;
    QRD::QRD_Index = QRD_Index;
    QRD::QRD_Max = QRD_Max;
    QRD::QRD_Min = QRD_Min; 
    QRD::QRD_Edge = (QRD_Max + QRD_Min)/2;
}

void QRD::update() {
    //update value
    digitalWrite(Select_0, QRD_Index&1);
    digitalWrite(Select_1, QRD_Index&2);
    digitalWrite(Select_2, QRD_Index&4);
    
    value = analogRead(QRD_port);
}

int QRD::getValue() {
    return value;
}

int QRD::getMax() {
    return QRD_Max;
}

int QRD::getMin() {
    return QRD_Min;
}

int QRD::getEdge() {
    return QRD_Edge;
}