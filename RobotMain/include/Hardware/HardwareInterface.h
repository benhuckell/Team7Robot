#ifndef __HARDWAREINTERFACE__
#define __HARDWAREINTERFACE__

#include "Arduino.h"
#include "Hardware/DriveMotor.h"
#include "Hardware/Encoder.h"
#include "Hardware/QRD.h"
#include "Hardware/ServoMotor.h"
#include "Hardware/ports.h"

class HardwareInterface {
  public:
     void update();

     static HardwareInterface* i();     
     static const int NUM_QRD_SENSORS = 8; //number of light sensors

     DriveMotor* LMotor;
     DriveMotor* RMotor;

     DriveMotor* WinchMotor;

     Encoder* LEncoder;
     Encoder* REncoder;
     Encoder* WinchEncoder;

     QRD* qrd0;
     QRD* qrd1;
     QRD* qrd2;
     QRD* qrd3;
     QRD* qrd4;
     QRD* qrd5;
     QRD* qrd6;
     QRD* qrd7;

     ServoMotor* clawMotor;

     QRD* QRD_Array[NUM_QRD_SENSORS];
     float QRD_Vals[NUM_QRD_SENSORS];
     int QRD_Prev[NUM_QRD_SENSORS];
     int QRD_RAW[NUM_QRD_SENSORS];
     int QRD_Thresh[NUM_QRD_SENSORS];
     int QRD_Max[NUM_QRD_SENSORS];
     int QRD_Min[NUM_QRD_SENSORS];

     float positionVector[NUM_QRD_SENSORS] = { -30.5 ,-18.0 ,-8.4, -1.75, 1.75, 8.4, 18.0, 30.5 };
     std::queue<float> errorHistory; //holds history of recorded line errors

     float getWeightedError();  
     
    void pushDriveSpeeds(int Lspeed, int Rspeed);
    void pushWinchSpeed(int speed);

     
      int motorcap(int input, int maxpower, int minpower);
      
  private:
     HardwareInterface();
     HardwareInterface(const HardwareInterface&);
     HardwareInterface& operator=(const HardwareInterface&);

     static HardwareInterface* myInstance;

      
  };

#endif
