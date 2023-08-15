#ifndef SYSTEM_H_
#define SYSTEM_H_

#include <logger.h>
#include <memory>

#include "TaskManager.h"
#include <BoardFinder.h>
#include <Display.h>
#include <configuration.h>

class System {
public:
  System();
  ~System();

  void setBoardConfig(BoardConfig const *const boardConfig);
  void setUserConfig(Configuration const *const userConfig);

  BoardConfig const *const   getBoardConfig() const;
  Configuration const *const getUserConfig() const;
  TaskManager               &getTaskManager();
  Display                   &getDisplay();
  bool                       isWifiOrEthConnected() const;
  void                       connectedViaEth(bool status);
  void                       connectedViaWifi(bool status);
  void                       gpsConnected(bool status);
  bool                       isGpsActive();
  void                       gpsLocation(double lat, double lng);
  double                     getGpsLat(void);
  double                     getGpsLong(void);
  logging::Logger           &getLogger();

private:
  BoardConfig const   *_boardConfig;
  Configuration const *_userConfig;
  TaskManager          _taskManager;
  Display              _display;
  bool                 _isEthConnected;
  bool                 _isWifiConnected;
  bool                 _isGpsConnected;
  double               _lat;
  double               _lng;
  logging::Logger      _logger;
};

#endif
