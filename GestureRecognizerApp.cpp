#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

#include "GestureRecognizer.h"

#include "I2Cdev.h"
#include "MPU6050.h"

MPU6050 accelgyro;
GestureRecognizer gestureRecognizer;

void setup() {
    // initialize device
    printf("Initializing I2C devices...\n");
    accelgyro.initialize();

    // verify connection
    printf("Testing device connections...\n");
    printf(accelgyro.testConnection() ? "MPU6050 connection successful\n" : "MPU6050 connection failed\n");
}

void recognizeGestures(void) {

  AccelerationData accelerationData;
  accelgyro.getAcceleration(&accelerationData.x, &accelerationData.y, &accelerationData.z);

  gestureRecognizer.addMotionData(accelerationData);

  Gesture* recognizedGesture = gestureRecognizer.recognize();
  if (!recognizedGesture) {
    return;
  }

  printf("Recognized gesture: %d\n", recognizedGesture->getId());
}

int main (int argc, char *argv[]) {

  setup();

  while (true) {
    recognizeGestures();
  }

	return 0;
}
