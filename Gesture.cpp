#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "Config.h"
#include "Gesture.h"

Gesture::Gesture() :
    id(0) {
}

void Gesture::setId(unsigned int id) {
  this->id = id;
}

unsigned int Gesture::getId() {
  return id;
}

void Gesture::addAccelerationData(AccelerationData& accelerationData) {

  sampler.addAccelerationData(accelerationData);
  if (!sampler.isReady()) {
    return;
  }

  addSampledDataToStore();
}

void Gesture::addSampledDataToStore(void) {
  if (!(sampler.hasRemainingData() || sampler.isReady())) {
    return;
  }

  AccelerationData& sample = sampler.getSample();
  //printf("sampled, x:%6hd, y:%6hd, z:%6hd\n", sample.x, sample.y, sample.z);

#ifdef USE_DATA_QUANTIZER

  AccelerationData quantizedData;
  quantizedData.x = dataQuantizer.quantize(sample.x);
  quantizedData.y = dataQuantizer.quantize(sample.y);
  quantizedData.z = dataQuantizer.quantize(sample.z);
  //printf("quantized, x:%6hd, y:%6hd, z:%6hd\n", quantizedData.x, quantizedData.y, quantizedData.z);

  accelerationDataStore.add(quantizedData);
#else
  sample.scale(100);
  //printf("scaled, x:%6hd, y:%6hd, z:%6hd\n", sample.x, sample.y, sample.z);

  accelerationDataStore.add(sample);
#endif

  sampler.prepareForUpdate();
}

void Gesture::import(void) {
  FILE *fp;
  char filename[10];

  AccelerationData accelerationData;

  sprintf(filename, "%d.uwv", id);
  printf("Importing %s ... ", filename);

  fp = fopen(filename, "r");

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
  addSampledDataToStore();

  fclose(fp);
  if (warnings) {
    printf("Warning, incorrectly formatted lines detected: %d\n", warnings);
    return;
  }

  printf("OK\n");
}

void Gesture::load(void) {
  char filename[10];
  sprintf(filename, "%d.ges", id);
  load(filename);
}

void Gesture::load(const char *filename) {
  FILE *fp;
  AccelerationData accelerationData;

  printf("Loading %s ... ", filename);

  fp = fopen(filename, "r");

  if (fp == NULL) {
    printf("Failed, cannot open file\n");
    return;
  }

  int warnings = 0;
  while (!feof(fp)) {
    int itemsRead = fscanf(fp, "%hd,%hd,%hd\n", &accelerationData.x, &accelerationData.y, &accelerationData.z);
    //printf("itemsRead: %d, x: %d, y: %d, z: %d\n", itemsRead, accelerationData.x, accelerationData.y, accelerationData.z);
    if (itemsRead != 3) {
      warnings++;
      break;
    }

    accelerationDataStore.add(accelerationData);
  }

  fclose(fp);
  if (warnings) {
    printf("Warning, incorrectly formatted lines detected: %d\n", warnings);
    return;
  }

  printf("OK\n");
}

void Gesture::save(void) {
  FILE *fp;
  char filename[10];

  sprintf(filename, "record.ges");
  printf("Saving %s ... ", filename);

  fp = fopen(filename, "w");

  if (fp == NULL) {
    printf("Failed, cannot write file\n");
    return;
  }

  AccelerationData *accelerationData = accelerationDataStore.getAccelerationData();
  for (unsigned int index = 0; index < accelerationDataStore.getTotalAccelerationDataItems(); index++) {

    fprintf(fp, "%hd,%hd,%hd\n", accelerationData[index].x, accelerationData[index].y, accelerationData[index].z);
  }

  fclose(fp);
  printf("OK\n");
}

bool Gesture::isValid(void) {
  return accelerationDataStore.getTotalAccelerationDataItems() > 0;
}

long Gesture::calculateDistanceBetween(Gesture& other) {
  if (!(isValid() && other.isValid())) {
    return INT_MAX;
  }

  AccelerationDataStore& otherAccelerationDataStore = other.getAccelerationDataStore();
  AccelerationData *otherAccelerationData = otherAccelerationDataStore.getAccelerationData();
  unsigned int otherTotalAccelerationDataItems = otherAccelerationDataStore.getTotalAccelerationDataItems();

  long* table = buildTable(otherTotalAccelerationDataItems);
  if (!table) {
    return INT_MAX;
  }

  unsigned int totalAccelerationDataItems = accelerationDataStore.getTotalAccelerationDataItems();

  long distance = calculateDTWDistance(otherAccelerationData, otherTotalAccelerationDataItems, totalAccelerationDataItems - 1,
      otherTotalAccelerationDataItems - 1, table);
  distance /= (totalAccelerationDataItems + otherTotalAccelerationDataItems);

  //printf("otherTotalAccelerationDataItems: %d, totalAccelerationDataItems: %d\n", otherTotalAccelerationDataItems, totalAccelerationDataItems);

  free(table);

  return distance;
}

long *Gesture::buildTable(unsigned int itemsInOtherGestureToCompare) {
  unsigned int totalAccelerationDataItems = accelerationDataStore.getTotalAccelerationDataItems();
  unsigned int tableSize = totalAccelerationDataItems * itemsInOtherGestureToCompare;
  unsigned int allocSize = tableSize * sizeof(long);

  //printf("tableSize: %d, allocSize: %d\n", tableSize, allocSize);
  long* table = (long*) malloc(allocSize);
  if (!table) {
    return NULL;
  }

  for (unsigned int index = 0; index < tableSize; index++) {
    table[index] = -1L;
  }

  return table;
}

AccelerationDataStore& Gesture::getAccelerationDataStore(void) {
  return accelerationDataStore;
}

/**
 * Due to the size of the numbers generated by this the acceleration data values should be small, x/y/z values of ~32768, cause infinite loops.
 */
long Gesture::calculateDTWDistance(AccelerationData *otherAccelerationData, unsigned int otherTotalAccelerationDataItems, int compareIndex,
    int otherCompareIndex, long *table) {

  //printf("compareIndex: %d, otherCompareIndex: %d\n", compareIndex, otherCompareIndex);
  AccelerationData *accelerationData = accelerationDataStore.getAccelerationData();

  if (compareIndex < 0 || otherCompareIndex < 0) {
    return INT_MAX;
  }

  int tableWidth = otherTotalAccelerationDataItems;
  long localDistance = 0;

  for (unsigned int axis = 0; axis < 3; axis++) {
    int16_t a = accelerationData[compareIndex].getAxisValue((Axis) axis);
    int16_t b = otherAccelerationData[otherCompareIndex].getAxisValue((Axis) axis);

    localDistance += ((a - b) * (a - b));
  }

  //printf("localDistance: %ld\n", localDistance);

  long sdistance = 0;
  long s1, s2, s3;

  if (compareIndex == 0 && otherCompareIndex == 0) {
    if (table[compareIndex * tableWidth + otherCompareIndex] < 0) {
      table[compareIndex * tableWidth + otherCompareIndex] = localDistance;
    }
    //printf("distance (a): %ld\n", table[compareIndex * tableWidth + otherCompareIndex]);
    return localDistance;
  }

  if (compareIndex == 0) {
    if (table[compareIndex * tableWidth + (otherCompareIndex - 1)] < 0)
      sdistance = calculateDTWDistance(otherAccelerationData, otherTotalAccelerationDataItems, compareIndex, otherCompareIndex - 1, table);
    else
      sdistance = table[compareIndex * tableWidth + otherCompareIndex - 1];
  } else if (otherCompareIndex == 0) {
    if (table[(compareIndex - 1) * tableWidth + otherCompareIndex] < 0)
      sdistance = calculateDTWDistance(otherAccelerationData, otherTotalAccelerationDataItems, compareIndex - 1, otherCompareIndex, table);
    else
      sdistance = table[(compareIndex - 1) * tableWidth + otherCompareIndex];
  } else {

    if (table[compareIndex * tableWidth + (otherCompareIndex - 1)] < 0)
      s1 = calculateDTWDistance(otherAccelerationData, otherTotalAccelerationDataItems, compareIndex, otherCompareIndex - 1, table);
    else
      s1 = table[compareIndex * tableWidth + (otherCompareIndex - 1)];

    if (table[(compareIndex - 1) * tableWidth + otherCompareIndex] < 0)
      s2 = calculateDTWDistance(otherAccelerationData, otherTotalAccelerationDataItems, compareIndex - 1, otherCompareIndex, table);
    else
      s2 = table[(compareIndex - 1) * tableWidth + otherCompareIndex];

    if (table[(compareIndex - 1) * tableWidth + otherCompareIndex - 1] < 0)
      s3 = calculateDTWDistance(otherAccelerationData, otherTotalAccelerationDataItems, compareIndex - 1, otherCompareIndex - 1, table);
    else
      s3 = table[(compareIndex - 1) * tableWidth + otherCompareIndex - 1];

    sdistance = s1 < s2 ? s1 : s2;
    sdistance = sdistance < s3 ? sdistance : s3;
  }
  table[compareIndex * tableWidth + otherCompareIndex] = localDistance + sdistance;
  //printf("distance (b): %ld\n", table[compareIndex * tableWidth + otherCompareIndex]);
  return table[compareIndex * tableWidth + otherCompareIndex];
}
