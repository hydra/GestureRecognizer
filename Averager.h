#ifndef AVERAGER_H
#define AVERAGER_H

#include "AccelerationData.h"

class Averager {

public:
  Averager();
  void reset(void);

  void addAccelerationData(AccelerationData& accelerationData);
  bool isFull(void);
  bool isEmpty(void);

  AccelerationData& getAverages(void);

private:
  AccelerationData buffer[SAMPLES_TO_AVERAGE];
  unsigned int totalSamplesInBuffer;

  AccelerationData averages;

};
#endif
