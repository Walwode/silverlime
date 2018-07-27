#ifndef PCREMOTE_H
#define PCREMOTE_H

#include <Scheduler.h>

class PcRemote : public Task {
  
public:
  PcRemote() {};
  ~PcRemote() {};

  static bool turnPcOn();
  static bool turnPcOff();

protected:
  void loop();
  void setup() {};
  
};
#endif
