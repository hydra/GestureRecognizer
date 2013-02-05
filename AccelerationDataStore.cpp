
#include "Config.h"
#include "AccelerationDataStore.h"

void AccelerationDataStore::add(AccelerationData& accelerationData) {

  if (totalAccelerationDataItems == MAX_ACCELERATION_ITEMS) {
    discardFirstItem();
  }

  this->accelerationData[totalAccelerationDataItems++] = accelerationData;
}


void AccelerationDataStore::discardFirstItem(void) {
  for (unsigned int index = 1; index < totalAccelerationDataItems; index++) {
    accelerationData[index-1] = accelerationData[index];
  }

  totalAccelerationDataItems--;
}
