#include <stdio.h>

#include "Config.h"
#include "Gesture.h"

Gesture::Gesture() :
    id(0), sampleFrequency(SAMPLE_FREQUENCY_HZ), addAccelerationDataCounter(0) {
  accelerationDataStep = sampleFrequency / 25;
}

void Gesture::setId(unsigned int id) {
  this->id = id;
}

unsigned int Gesture::getId() {
  return id;
}

void Gesture::addAccelerationData(AccelerationData& accelerationData) {
  addAccelerationDataCounter++;
  if (!shouldAddAccelerationData()) {
    return;
  }

  averager.addAccelerationData(accelerationData);
  if (!averager.isFull()) {
    return;
  }

  addAveragedDataToStore();
}

void Gesture::addAveragedDataToStore(void) {
  if (averager.isEmpty()) {
    return;
  }

  AccelerationData& averagedData = averager.getAverages();

  //printf("averaged, x:%6hd, y:%6hd, z:%6hd\n", averagedData.x, averagedData.y, averagedData.z);
#ifdef USE_DATA_QUANTIZER

  AccelerationData quantizedData;
  quantizedData.x = dataQuantizer.quantize(averagedData.x);
  quantizedData.y = dataQuantizer.quantize(averagedData.y);
  quantizedData.z = dataQuantizer.quantize(averagedData.z);
  printf("quantized, x:%6hd, y:%6hd, z:%6hd\n", quantizedData.x, quantizedData.y, quantizedData.z);

  accelerationDataStore.add(quantizedData);
#else
  accelerationDataStore.add(averagedData);
#endif

  averager.reset();
}

bool Gesture::shouldAddAccelerationData(void) {
  return ((addAccelerationDataCounter - 1) % accelerationDataStep == 0);
}

void Gesture::load(void) {
  FILE *fp;
  char fname[10];

  AccelerationData accelerationData;

  sprintf(fname, "%d.uwv", id);
  printf("Loading %s..", fname);

  fp = fopen(fname, "r");

  if (fp == NULL) {
    printf("Failed, cannot open file\n");
    return;
  }

  int warnings = 0;
  while (!feof(fp)) {
    int itemsRead = fscanf(fp, "%hd %hd %hd \n", &accelerationData.x, &accelerationData.y, &accelerationData.z);
    //printf("itemsRead: %d, x: %d, y: %d, z: %d\n", itemsRead, accelerationData.x, accelerationData.y, accelerationData.z);
    if (itemsRead != 3) {
      warnings++;
      break;
    }

    addAccelerationData(accelerationData);
  }
  addAveragedDataToStore();

  fclose(fp);
  if (warnings) {
    printf("Warning, incorrectly formatted lines detected: %d\n", warnings);
    return;
  }

  printf("OK\n");
}
