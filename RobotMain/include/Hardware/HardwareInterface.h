#ifndef __HARDWAREINTERFACE__
#define __HARDWAREINTERFACE__

#include "Hardware/DriveMotor.h"
#include "Hardware/Encoder.h"
#include "Hardware/QRD.h"
#include "Hardware/ServoMotor.h"
#include "stateController.h"

#define LMOTOR_FORWARDS PA_0 //motor ports
#define LMOTOR_BACKWARDS PA_1
#define RMOTOR_FORWARDS PA_2
#define RMOTOR_BACKWARDS PA_3

#define QRD_IN PA_4 //QRD read port

#define Select_0 PB12 //mux selects
#define Select_1 PB13
#define Select_2 PB14

class HardwareInterface {
   public:
      void update();

      static HardwareInterface* i();      
      static const int NUM_QRD_SENSORS = 4; //number of light sensors

      //extern Adafruit_SSD1306 display;

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
      int QRD_Vals[NUM_QRD_SENSORS];
      int QRD_Maxims[NUM_QRD_SENSORS];
      int QRD_Mins[NUM_QRD_SENSORS];
      int QRD_Edge[NUM_QRD_SENSORS];
      //HardwareInstance::instance()->readQRD(QRDId_1);

      unsigned long previousMillis = 0;
      unsigned long currentMillis = millis();

      float duration;

   private:
      HardwareInterface();
      HardwareInterface(const HardwareInterface&);
      HardwareInterface& operator=(const HardwareInterface&);

      static HardwareInterface* myInstance;
   };

#endif