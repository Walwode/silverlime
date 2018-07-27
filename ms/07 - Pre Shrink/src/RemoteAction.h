#ifndef REMOTEACTION_H
#define REMOTEACTION_H

class RemoteAction {
  
  int action;
  int duration;

public:
  RemoteAction(int action, int duration = 100) {
    this->action = action;
    this->duration = duration;
  }
  ~RemoteAction();
  
  int getAction() {
    return action;
  }
  
  int getDuration() {
    return duration;
  }
};
#endif

