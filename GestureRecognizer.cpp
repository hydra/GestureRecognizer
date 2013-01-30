
#include <stdio.h>
#include "GestureRecognizer.h"

GestureRecognizer::GestureRecognizer() : readyToRecognize(false) {
}

void GestureRecognizer::addMotionData(AccelerationData& accelerationData) {

  printf("a/g: %6hd %6hd %6hd\n", accelerationData.x, accelerationData.y, accelerationData.z);

  averager.addMotionData(accelerationData);
  if (!averager.isFull()) {
    return;
  }

  AccelerationData& averagedData = averager.getAverages();

  printf("average a/g: %6hd %6hd %6hd\n", averagedData.x, averagedData.y, averagedData.z);

  accelerationDataStore.add(averagedData);
  averager.reset();

  readyToRecognize = true;
}

Gesture *GestureRecognizer::recognize(void) {
  if (!readyToRecognize) {
    return NULL;
  }

  readyToRecognize = false;

  // TODO recognize gesture
  printf("recognizing...");

  printf("not recognized\n");

  return NULL;
}
