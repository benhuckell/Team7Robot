#pragma once
#include "Arduino.h"

#define Select_0 PA_15 //mux selects
#define Select_1 PB_3
#define Select_2 PB_4

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
};
