#pragma once
#include "Arduino.h"
#include <queue>

#define Select_0 PB12 //mux selects
#define Select_1 PB13
#define Select_2 PB14

class QRD {
   public:
      QRD(PinName QRD_port, int QRD_Index, int QRD_Max, int QRD_Min);
      void update(); //updates value,max,min

      int getValue();
      int getMax();
      int getMin();
      int getEdge();

   private:
      int QRD_Index;
      PinName QRD_port;
      int value;
      int QRD_Max;
      int QRD_Min;
      int QRD_Edge;
      std::queue<int> valHistory;
      std::queue<int> valCopy;
      const int VAL_HISTORY_SIZE = 10; 
};
