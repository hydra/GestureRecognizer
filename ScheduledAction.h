#ifndef SCHEDULEDACTION_H
#define SCHEDULEDACTION_H

#include <sys/time.h>

class ScheduledAction {
public:
  void reset();
  void setDelayMicros(unsigned long delayMicros);
  void setDelayMillis(unsigned long delayMillis);
  unsigned long getLateBy(void);
  int getMissedActions(void);
  bool isActionDue(void);

private:
  timeval tempTime;
  unsigned long micros(void);

  unsigned long nextActionAt;
  unsigned long delayMicros;
  unsigned int missedActions;
  unsigned long lateBy;

};

#endif
