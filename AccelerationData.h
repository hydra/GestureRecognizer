#ifndef ACCELERATIONDATA_H
#define ACCELERATIONDATA_H

#include <stdint.h>

enum Axis { AXIS_X, AXIS_Y, AXIS_Z };

class AccelerationData {
public:
  int16_t x, y, z;

  void setAxisValue(Axis axis, int16_t value) {
    switch(axis) {
      case AXIS_X:
        x = value;
      break;
      case AXIS_Y:
        y = value;
      break;
      case AXIS_Z:
        z = value;
      break;
    }
  }

  int16_t getAxisValue(Axis axis) {
    switch(axis) {
      case AXIS_X:
        return x;
      break;
      case AXIS_Y:
        return y;
      break;
      case AXIS_Z:
        return z;
      break;
    }
    return 0;
  }

  void scale(short divider) {
    x = x / divider;
    y = y / divider;
    z = z / divider;
  }
};

#endif
