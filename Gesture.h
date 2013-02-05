#ifndef GESTURE_H
#define GESTURE_H

#include "AccelerationData.h"
#include "AccelerationDataStore.h"
#include "Averager.h"
#include "DataQuantizer.h"
#include "Gesture.h"

class Gesture {
public:
  Gesture();
  void setId(unsigned int id);
  unsigned int getId(void);

  void load(void);

  void addAccelerationData(AccelerationData& accelerationData);

private:
  unsigned int id;
  Averager averager;
  AccelerationDataStore accelerationDataStore;
#ifdef USE_DATA_QUANTIZER
  DataQuantizer dataQuantizer;
#endif

  unsigned int sampleFrequency;
  unsigned int addAccelerationDataCounter;
  unsigned int accelerationDataStep;

  bool shouldAddAccelerationData(void);
  void addAveragedDataToStore(void);

};

#endif
