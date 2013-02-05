
#include <stdio.h>

#include "Config.h"
#include "Averager.h"

Averager::Averager() : totalSamplesInBuffer(0) {
  reset();
}

void Averager::reset(void) {
  totalSamplesInBuffer = 0;
}

void Averager::addAccelerationData(AccelerationData& accelerationData) {
  if (isFull()) {
    return;
  }

  buffer[totalSamplesInBuffer++] = accelerationData;
}

bool Averager::isFull(void) {
  return SAMPLES_TO_AVERAGE == totalSamplesInBuffer;
}

bool Averager::isEmpty(void) {
  return totalSamplesInBuffer == 0;
}

AccelerationData& Averager::getAverages(void) {

  long int totalX, totalY, totalZ;

  totalX = 0;
  totalY = 0;
  totalZ = 0;

  for(unsigned int index = 0; index < totalSamplesInBuffer; index++) {
    totalX += buffer[index].x;
    totalY += buffer[index].y;
    totalZ += buffer[index].z;
  }
  averages.x = totalX * 1.0 / totalSamplesInBuffer;
  averages.y = totalY * 1.0 / totalSamplesInBuffer;
  averages.z = totalZ * 1.0 / totalSamplesInBuffer;

  return averages;
}
