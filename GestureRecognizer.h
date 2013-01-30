
#ifndef GESTURERECOGNIZER_H
#define GESTURERECOGNIZER_H

#include "AccelerationData.h"
#include "AccelerationDataStore.h"
#include "Averager.h"

#include "Gesture.h"

class GestureRecognizer {

public:
  GestureRecognizer();
  void addMotionData(AccelerationData& accelerationData);
  Gesture *recognize(void);

private:
  Averager averager;
  AccelerationDataStore accelerationDataStore;
  bool readyToRecognize;
};

#endif
