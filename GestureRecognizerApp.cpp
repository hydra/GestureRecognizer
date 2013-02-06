#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

#include "Config.h"
#include "GestureRecognizer.h"
#include "ScheduledAction.h"

#include "I2Cdev.h"
#include "MPU6050.h"

MPU6050 accelgyro;
GestureRecognizer gestureRecognizer;

ScheduledAction sampleAccelerationDataAction;
ScheduledAction recognizeGestureAction;

void setup() {

    // initialize device
    printf("Initializing I2C devices...\n");
    accelgyro.initialize();

    // verify connection
    printf("Testing device connections...\n");
    printf(accelgyro.testConnection() ? "MPU6050 connection successful\n" : "MPU6050 connection failed\n");

    printf("Loading recorded gestures\n");
    gestureRecognizer.loadGestures();

    sampleAccelerationDataAction.setDelayMicros((1000L * 1000L) / SAMPLE_FREQUENCY_HZ);
    sampleAccelerationDataAction.reset();
}

void sampleAccelerationData(void) {

  AccelerationData accelerationData;
  accelgyro.getAcceleration(&accelerationData.x, &accelerationData.y, &accelerationData.z);

  gestureRecognizer.addAccelerationData(accelerationData);
}

void recognizeGesture(void) {
  Gesture* recognizedGesture = gestureRecognizer.recognize();
  if (!recognizedGesture) {
    return;
  }

  printf("\nRecognized gesture: %d\n\n", recognizedGesture->getId());
}


void replayAndRecognize() {

  gestureRecognizer.load("replay.ges");
  printf("Recognizing...\n");
  recognizeGesture();
}

void recordAndRecognize(bool continuousMode) {

  unsigned long recognizeDelayMicros = (1000L * 1000L) * 0.25;

  if (!continuousMode) {
    recognizeDelayMicros = (1000L * 1000L) * 1;
  }
  recognizeGestureAction.setDelayMicros(recognizeDelayMicros);
  recognizeGestureAction.reset();

  printf("Recording...\n");
  while (true) {
    if (sampleAccelerationDataAction.isActionDue()) {
      sampleAccelerationData();
    }

    if (recognizeGestureAction.isActionDue()) {
      printf("Recognizing...\n");
      recognizeGesture();
      if (!continuousMode) {
        break;
      }
    }
  }
}

int main(int argc, char *argv[]) {

  setup();

  printf("args: %d\n", argc);
  for (int index = 0; index < argc; index++) {
    printf("%s\n", argv[index]);
  }
  if (argc == 2 && strcasecmp(argv[1], "REPLAY") == 0) {
    replayAndRecognize();
    return 0;
  }

  bool continuousMode = false;
  if (argc == 2 && strcasecmp(argv[1], "CONTINUOUS") == 0) {
    continuousMode = true;
  }

  recordAndRecognize(continuousMode);
  return 0;
}
