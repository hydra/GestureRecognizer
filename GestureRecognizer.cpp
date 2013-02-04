
#include <stdio.h>

#include "Config.h"
#include "GestureRecognizer.h"

GestureRecognizer::GestureRecognizer() : readyToRecognize(false), sampleFrequency(SAMPLE_FREQUENCY_HZ), addMotionDataCounter(0) {
  motionDataStep = sampleFrequency / 25;
}

void GestureRecognizer::addMotionData(AccelerationData& accelerationData) {

  addMotionDataCounter++;
  if (!shouldAddMotionData()) {
    return;
  }

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

bool GestureRecognizer::shouldAddMotionData(void) {
  return (addMotionDataCounter % motionDataStep == 0);
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
