#ifndef AVERAGER_H
#define AVERAGER_H

#include "AccelerationData.h"

#define SAMPLE_BUFFER_SIZE (SAMPLE_WINDOW + SAMPLE_STEP)

/**
 * Generate a sample from available data
 *
 * This sampler implements a FIFO buffer of AccelerationData items.
 *
 * Usage:
 * The caller is expected to repeatedly call addAccelerationData() followed by isReady()
 * When isReady() returns true the caller can call getSample() followed by a call to prepareForUpdate()
 * Calling prepareForUpdate() removes unneeded items from the FIFO buffer and allows new items to be added.
 * If hasRemainingData() returns true it indicates that getSample() call be called.
 *
 * hasRemainingData() can return true if the amount of items added to the buffer is less than the sample window
 * This can occur, for instance, when using a data set size that is not a multiple of the sample window
 */

class Sampler {

public:
  Sampler();

  void addAccelerationData(AccelerationData& accelerationData);
  bool isReady(void);
  bool hasRemainingData(void);

  AccelerationData& getSample(void);
  void prepareForUpdate(void);

private:
  AccelerationData buffer[SAMPLE_BUFFER_SIZE];
  unsigned int totalSamplesInBuffer;

  AccelerationData sample;

  bool isFull(void);
  bool isEmpty(void);

};
#endif
