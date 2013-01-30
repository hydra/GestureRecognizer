#ifndef GESTURE_H
#define GESTURE_H

class Gesture {
public:
  Gesture();
  void setId(unsigned int id);
  unsigned int getId(void);

private:
  unsigned int id;
};

#endif
