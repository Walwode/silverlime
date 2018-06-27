#ifndef REMOTEACTION_H
#define REMOTEACTION_H

class RemoteAction {
  
  int action;
  int duration;

public:
  // RemoteAction(int action) : RemoteAction(action, 100) {};
  RemoteAction(int action) {
    this->action = action;
    this->duration = 100;
  }
  RemoteAction(int action, int duration) {
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

