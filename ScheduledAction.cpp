#include <stdio.h>

#include "ScheduledAction.h"

void ScheduledAction::reset() {
  nextActionAt = micros() + delayMicros;
  missedActions = 0;
}

void ScheduledAction::setDelayMicros(unsigned long delayMicros) {
  this->delayMicros = delayMicros;
}

void ScheduledAction::setDelayMillis(unsigned long delayMillis) {
  this->delayMicros = delayMillis * 1000L;
}

unsigned long ScheduledAction::getLateBy(void) {
  return lateBy;
}

int ScheduledAction::getMissedActions(void) {
  return missedActions;
}

bool ScheduledAction::isActionDue(void) {
  unsigned long now = micros();
  //printf("now: %lu\n", now);
  long signedDiff = now - nextActionAt;
  //printf("signedDiff: %ld\n", signedDiff);

  if (signedDiff < 0L) {
    missedActions = 0;
    lateBy = 0;
    return false;
  }

  missedActions = signedDiff / delayMicros;
  //printf("missedActions: %u\n", missedActions);

  lateBy = signedDiff % delayMicros;
  //printf("lateBy: %lu\n", lateBy);

  nextActionAt = nextActionAt + (delayMicros - lateBy) + (delayMicros * missedActions);
  //printf("nextActionAt: %lu\n", nextActionAt);

  return true;
}

unsigned long ScheduledAction::micros(void) {
  gettimeofday(&tempTime, NULL);
  return (tempTime.tv_sec * (1000L * 1000L)) + tempTime.tv_usec;
}
