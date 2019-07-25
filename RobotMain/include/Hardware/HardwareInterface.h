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


#define WINCH_UP PA_2 //winch inputs
#define WINCH_DOWN PA_3

#define WINCH_ENC_1 PA5
#define WINCH_ENC_2 PA4

#define LENCODER_1 PB15 //encoder inputs
#define LENCODER_2 PA8
#define RENCODER_1 PA11
#define RENCODER_2 PA12
#define LENCODER_3 PB10
#define RENCODER_3 PB1
#define LIM_SWIT_PIN PC15

#define CLAW_SERVO PB_8

#define QRD_IN PA_6 //QRD read port

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
      int QRD_RAW[NUM_QRD_SENSORS];
      int QRD_Thresh[NUM_QRD_SENSORS];
      int QRD_Max[NUM_QRD_SENSORS];
      int QRD_Min[NUM_QRD_SENSORS];
      int Winch_tick_target=0;
      bool rock;

      unsigned long previousMillis = 0;
      unsigned long currentMillis = millis();
      bool robotWasBumped();
      bool robotHitPost();
      void moveIntake();
      void clawSetPos(int clawAngle);
      bool checkForRock();

   private:
      HardwareInterface();
      HardwareInterface(const HardwareInterface&);
      HardwareInterface& operator=(const HardwareInterface&);
      bool timer(unsigned int preset);
      float lastLSpeed;
      float lastRSpeed;
      static HardwareInterface* myInstance;
      const float bumpThresholdVal = 10.0;
      const float postThresholdVal = 0.5;
      static const int POST_1_TICKS = 1;
      static const int POST_2_TICKS = 1;
      static const int POST_3_TICKS = 1;
      static const int POST_4_TICKS = 1;
      static const int POST_5_TICKS = 1;
      static const int POST_6_TICKS = 1;
      float Winch_P_gain=1;

      //can hard code these values
      int clawFullyClosed;
      int clawFullyOpen;
      int clawWithRock;

      int clawCurrentAngle=clawFullyOpen;


   };


#endif