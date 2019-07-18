#ifndef __HARDWAREINTERFACE__
#define __HARDWAREINTERFACE__

#include "Arduino.h"
#include "Hardware/DriveMotor.h"
#include "Hardware/Encoder.h"
#include "Hardware/QRD.h"
#include "Hardware/ServoMotor.h"

#define LMOTOR_FORWARDS PA_0 //motor ports
#define LMOTOR_BACKWARDS PA_1
#define RMOTOR_FORWARDS PA_2
#define RMOTOR_BACKWARDS PA_3

#define PUSH_BUTTON_1 PB4
#define PUSH_BUTTON_2 PB5
#define CONTROL_POT_1 PA_4
#define CONTROL_POT_2 PA_5

/*
#define WINCH_UP PA_8 //winch inputs
#define WINCH_DOWN PA_9
*/
#define LENCODER_1 PB1 //encoder inputs
#define LENCODER_2 PB10
#define RENCODER_1 PB1
#define RENCODER_2 PB10

//#define CLAW_SERVO PA_10

#define QRD_IN PA_6 //QRD read port

class HardwareInterface {
   public:
      void update();

      static HardwareInterface* i();      
      static const int NUM_QRD_SENSORS = 4; //number of light sensors

      DriveMotor* LMotor;
      DriveMotor* RMotor;

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
      float QRD_Vals[NUM_QRD_SENSORS];
      int QRD_Thresh[NUM_QRD_SENSORS];
      int QRD_Max[NUM_QRD_SENSORS];
      int QRD_Min[NUM_QRD_SENSORS];

      unsigned long previousMillis = 0;
      unsigned long currentMillis = millis();
      bool robotWasBumped();
      bool robotHitPost();

   private:
      HardwareInterface();
      HardwareInterface(const HardwareInterface&);
      HardwareInterface& operator=(const HardwareInterface&);
      bool timer(unsigned int preset);
      float lastLSpeed;
      float lastRSpeed;
      static HardwareInterface* myInstance;
      const float bumpThresholdVal = 1.0;
      const float postThresholdVal = 0.5;
   };

#endif