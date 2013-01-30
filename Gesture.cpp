
#include "Gesture.h"

Gesture::Gesture() : id(0) {

}

void Gesture::setId(unsigned int id) {
  this->id = id;
}

unsigned int Gesture::getId() {
  return id;
}
