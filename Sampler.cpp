
#include <stdio.h>

#include "Config.h"
#include "Sampler.h"

Sampler::Sampler() : totalSamplesInBuffer(0) {
}

void Sampler::prepareForUpdate(void) {

  for (unsigned int index = SAMPLE_STEP; index < totalSamplesInBuffer; index++) {
    buffer[index - SAMPLE_STEP].x = buffer[index].x;
    buffer[index - SAMPLE_STEP].y = buffer[index].y;
    buffer[index - SAMPLE_STEP].z = buffer[index].z;
  }
  totalSamplesInBuffer -= SAMPLE_STEP;
}

void Sampler::addAccelerationData(AccelerationData& accelerationData) {
  if (isFull()) {
    return;
  }

  buffer[totalSamplesInBuffer++] = accelerationData;
}

bool Sampler::isReady(void) {
  return totalSamplesInBuffer % SAMPLE_WINDOW == 0;
}

bool Sampler::isFull(void) {
  return totalSamplesInBuffer == SAMPLE_BUFFER_SIZE;
}

bool Sampler::isEmpty(void) {
  return totalSamplesInBuffer == 0;
}

bool Sampler::hasRemainingData(void) {
  return !isReady() && !isEmpty();
}

AccelerationData& Sampler::getSample(void) {

  long int totalX, totalY, totalZ;

  totalX = 0;
  totalY = 0;
  totalZ = 0;

  unsigned int window = SAMPLE_WINDOW;
  if (window > totalSamplesInBuffer) {
    window = totalSamplesInBuffer;
  }

  for(unsigned int index = 0; index < SAMPLE_WINDOW; index++) {
    totalX += buffer[index].x;
    totalY += buffer[index].y;
    totalZ += buffer[index].z;
  }
  sample.x = totalX * 1.0 / SAMPLE_WINDOW;
  sample.y = totalY * 1.0 / SAMPLE_WINDOW;
  sample.z = totalZ * 1.0 / SAMPLE_WINDOW;

  return sample;
}
