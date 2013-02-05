#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

#include "Config.h"
#include "GestureRecognizer.h"

#include "I2Cdev.h"
#include "MPU6050.h"

MPU6050 accelgyro;
GestureRecognizer gestureRecognizer;


#include <sys/time.h>

class ScheduledAction {
public:
  void reset() {
    nextActionAt = micros();
    missedActions = 0;
  }

  void setDelayMicros(unsigned long delayMicros) {
    this->delayMicros = delayMicros;
  }

  void setDelayMillis(unsigned long delayMillis) {
    this->delayMicros = delayMillis * 1000L;
  }

  unsigned long getLateBy(void) {
    return lateBy;
  }

  int getMissedActions(void) {
    return missedActions;
  }

  bool isActionDue(void) {
    unsigned long now = micros();
    //printf("now: %lu\n", now);
    long signedDiff = now - nextActionAt;
    //printf("signedDiff: %ld\n", signedDiff);

    if (signedDiff < 0L) {
      missedActions = 0;
      lateBy = 0;
      return false;
    }

    missedActions = signedDiff / delayMicros;
    //printf("missedActions: %u\n", missedActions);

    lateBy = signedDiff % delayMicros;
    //printf("lateBy: %lu\n", lateBy);

    nextActionAt = nextActionAt + (delayMicros - lateBy) + (delayMicros * missedActions);
    //printf("nextActionAt: %lu\n", nextActionAt);

    return true;
  }

private:
  timeval tempTime;
  unsigned long micros(void) {
    gettimeofday(&tempTime, NULL);
    return (tempTime.tv_sec * (1000L * 1000L)) + tempTime.tv_usec;
  }


  unsigned long nextActionAt;
  unsigned long delayMicros;
  unsigned int missedActions;
  unsigned long lateBy;

};

ScheduledAction sampleAccelerationDataAction;

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

void recognizeGestures(void) {

  AccelerationData accelerationData;
  accelgyro.getAcceleration(&accelerationData.x, &accelerationData.y, &accelerationData.z);

  gestureRecognizer.addAccelerationData(accelerationData);

  Gesture* recognizedGesture = gestureRecognizer.recognize();
  if (!recognizedGesture) {
    return;
  }

  printf("Recognized gesture: %d\n", recognizedGesture->getId());
}

int main (int argc, char *argv[]) {

  setup();

  while (true) {
    if (!sampleAccelerationDataAction.isActionDue()) {
      continue;
    }

    recognizeGestures();
  }

	return 0;
}
