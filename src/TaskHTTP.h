#ifndef TASK_HTTP_H_
#define TASK_HTTP_H_

#include <WiFiServer.h>
#include <TaskManager.h>

class HTTPTask : public Task {
public:
  HTTPTask();
  virtual ~HTTPTask();

  virtual bool setup(System &system) override;
  virtual bool loop(System &system) override;

private:
  WiFiServer _wifiServer;
  bool       _beginCalled;
};

#endif
