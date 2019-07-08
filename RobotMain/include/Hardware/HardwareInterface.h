#ifndef __HARDWAREINTERFACE__
#define __HARDWAREINTERFACE__

#include "Arduino.h"
#include "libs/Adafruit_SSD1306.h"
#include "Hardware/DriveMotor.h"
#include "Hardware/Encoder.h"
#include "Hardware/QRD.h"
#include "Hardware/ServoMotor.h"

#define LMOTOR_FORWARDS_1 PA_0 //motor ports
#define LMOTOR_BACKWARDS_1 PA_1
#define LMOTOR_FORWARDS_2 PA_2
#define LMOTOR_BACKWARDS_2 PA_3
#define RMOTOR_FORWARDS_1 PA_4
#define RMOTOR_BACKWARDS_1 PA_5
#define RMOTOR_FORWARDS_2 PA_6
#define RMOTOR_BACKWARDS_2 PA_7

#define WINCH_UP PA_8 //winch inputs
#define WINCH_DOWN PA_9

#define LENCODER_1 PB_5 //encoder inputs
#define LENCODER_2 PB_6
#define RENCODER_1 PB_7
#define RENCODER_2 PB_8

#define CLAW_SERVO PA_10

#define QRD_IN PA_4 //QRD read port

#define Select_0 PA_15 //mux selects
#define Select_1 PB_3
#define Select_2 PB_4

extern Adafruit_SSD1306 display;

class HardwareInterface {
   public:
      void update();

      static HardwareInterface* i();      
      static const int NUM_QRD_SENSORS = 4; //number of light sensors

      DriveMotor* LMotor1;
      DriveMotor* LMotor2;
      DriveMotor* RMotor1;
      DriveMotor* RMotor2;

      DriveMotor* WinchMotor;

      Encoder* LEncoder;
      Encoder* REncoder;

      QRD* qrd0;
      QRD* qrd1;
      QRD* qrd2;
      QRD* qrd3;
      QRD* qrdLeft;
      QRD* qrdRight;

      ServoMotor* clawMotor;

      QRD* QRD_Array[NUM_QRD_SENSORS];
      int QRD_Vals[NUM_QRD_SENSORS];
      int QRD_Maxims[NUM_QRD_SENSORS];
      int QRD_Mins[NUM_QRD_SENSORS];
      int QRD_Edge[NUM_QRD_SENSORS];
      //HardwareInstance::instance()->readQRD(QRDId_1);

      std::clock_t start;
      float duration;

   private:
      HardwareInterface();
      HardwareInterface(const HardwareInterface&);
      HardwareInterface& operator=(const HardwareInterface&);

      static HardwareInterface* myInstance;
   };

#endif