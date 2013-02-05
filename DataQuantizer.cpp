#include "Config.h"
#include "DataQuantizer.h"

int16_t DataQuantizer::quantize(int16_t value) {
  int16_t result;

  if (value > 10) {
    if (value > 20) {
      result = 16;
    } else {
      result = 10 + (value - 10) / 10 * 5;
    }

  } else if (value < -10) {
    if (value < -20) {
      result = -16;
    } else {
      result = -10 + (value + 10) / 10 * 5;
    }
  } else {
    result = value;
  }

  return result;
}
