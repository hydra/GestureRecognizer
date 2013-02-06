
#include <stdio.h>
#include <limits.h>

#include "Config.h"
#include "GestureRecognizer.h"

GestureRecognizer::GestureRecognizer() {
  for (int index = 0; index < MAX_GESTURES; index++) {
    Gesture& gesture = gestures[index];
    gesture.setId(index);
  }
}

void GestureRecognizer::load(const char *replayFilename) {
  possibleGesture.load(replayFilename);
}

void GestureRecognizer::loadGestures(void) {
  for (int index = 0; index < MAX_GESTURES; index++) {
    Gesture& gesture = gestures[index];
    gesture.load();
  }
}

void GestureRecognizer::addAccelerationData(AccelerationData& accelerationData) {

  //printf("raw, x:%6hd, y:%6hd, z: %6hd\n", accelerationData.x, accelerationData.y, accelerationData.z);
  possibleGesture.addAccelerationData(accelerationData);
}

Gesture *GestureRecognizer::recognize(void) {

  possibleGesture.save();

  long shortestDistance = INT_MAX;
  long longestDistance = 0;
  Gesture *bestMatch = NULL;

  long totalDistance = 0;
  unsigned int gesturesChecked = 0;

  for (unsigned int index = 0; index < MAX_GESTURES; index++) {
    Gesture& gesture = gestures[index];
    if (!gesture.isValid()) {
      continue;
    }
    long distance = possibleGesture.calculateDistanceBetween(gesture);

    if (distance < shortestDistance) {
      shortestDistance = distance;
      bestMatch = &gesture;
    }

    if (distance > longestDistance) {
      longestDistance = distance;
    }

    totalDistance += distance;
    gesturesChecked++;

    printf("index: %u, distance: %ld\n", index, distance);
  }

  long averageDistance = totalDistance / gesturesChecked;
  long variance = longestDistance - shortestDistance;

  printf("averageDistance: %ld, shortestDistance: %ld, longestDistance: %ld, variance: %ld\n", averageDistance, shortestDistance, longestDistance, variance);

  if (shortestDistance > averageDistance / 2) {
    printf("almost matched %d\n", bestMatch->getId());
    return NULL;
  }

  return bestMatch;
}
