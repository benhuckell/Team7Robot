#pragma once
#include "Arduino.h"
#include <queue>

#define Select_0 PB12 //mux selects
#define Select_1 PB13
#define Select_2 PB14

class QRD {
   public:
      QRD(PinName QRD_port, int QRD_Index, int QRD_Thresh, int QRD_Min, int QRD_Max);
      void update(); //updates value,max,min

      int getValue();
      int getThresh();
      int getMin();
      int getMax();

   private:
      bool states[8][3] = {{LOW,LOW,LOW},{LOW,LOW,HIGH},{LOW,HIGH,LOW},{LOW,HIGH,HIGH},{HIGH,LOW,LOW},{HIGH,LOW,HIGH},{HIGH,HIGH,LOW},{HIGH,HIGH,HIGH}};
      int QRD_Index;
      PinName QRD_port;
      int value;
      int QRD_Thresh;
      int QRD_Max;
      int QRD_Min;
      std::queue<int> valHistory;
      std::queue<int> valCopy;
      const int VAL_HISTORY_SIZE = 10; 
};