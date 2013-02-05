
#include <stdio.h>

#include "Config.h"
#include "GestureRecognizer.h"

GestureRecognizer::GestureRecognizer() {
}

void GestureRecognizer::loadGestures(void) {
  for (int index = 0; index < MAX_GESTURES; index++) {
    Gesture& gesture = gestures[index];
    gesture.setId(index);
    gesture.load();
  }
}

void GestureRecognizer::addAccelerationData(AccelerationData& accelerationData) {

  printf("raw, x:%6hd, y:%6hd, z: %6hd\n", accelerationData.x, accelerationData.y, accelerationData.z);
  possibleGesture.addAccelerationData(accelerationData);
}

Gesture *GestureRecognizer::recognize(void) {

  // TODO recognize gesture
  //printf("recognizing...");

  //printf("not recognized\n");

  return NULL;
}
