
#ifndef GESTURERECOGNIZER_H
#define GESTURERECOGNIZER_H

#include "AccelerationData.h"
#include "Gesture.h"

class GestureRecognizer {

public:
  GestureRecognizer();
  void load(const char *replayFilename);
  void loadGestures(void);

  void addAccelerationData(AccelerationData& accelerationData);
  Gesture *recognize(void);

private:
  Gesture gestures[MAX_GESTURES];
  Gesture possibleGesture;
};

#endif
