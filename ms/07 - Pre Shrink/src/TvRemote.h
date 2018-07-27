#ifndef TVREMOTE_H
#define TVREMOTE_H

#include <Scheduler.h>

class TvRemote : public Task {
  
public:
  TvRemote() {};
  ~TvRemote() {};

  static bool turnTvOn();
  static bool turnTvOff();
  static bool turnNetflixOn();
  static bool turnNetflixOff();

protected:
  void loop();
  void setup() {};
  
};
#endif
