#include <logger.h>

#include "Task.h"
#include "TaskHTTP.h"
#include "project_configuration.h"

HTTPTask::HTTPTask() : Task(TASK_HTTP, TaskHttp), _beginCalled(false) {
}

HTTPTask::~HTTPTask() {
}

bool HTTPTask::setup(System &system) {
  return true;
}

bool HTTPTask::loop(System &system) {
  if (!_beginCalled) {
    if (!system.isWifiOrEthConnected()) {
      return false;
    }

    system.getLogger().log(logging::LoggerLevel::LOGGER_LEVEL_INFO, getName(), "Starting Web Server");
    _wifiServer = WiFiServer(80);
    _wifiServer.begin();
    _beginCalled = true;
  }

  WiFiClient client = _wifiServer.available();

  if (client) {

    system.getLogger().log(logging::LoggerLevel::LOGGER_LEVEL_INFO, getName(), "New Client");

    bool currentLineIsBlank = true;
    String request;

    unsigned long currentTime = millis();
    unsigned long previousTime = currentTime;

    while (client.connected() && currentTime - previousTime <= 1000) {
      currentTime = millis();

      if (client.available()) {

        char c = client.read();
        request += c;

        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply

        if (c == '\n' && currentLineIsBlank) {

          // Only support GET /
          // system.getLogger().log(logging::LoggerLevel::LOGGER_LEVEL_INFO, getName(), "Request %s", request.c_str());
          if (request.indexOf("GET / HTTP") == -1) {
            client.println("HTTP/1.1 404");
            client.println();
            break;
          }

          client.println("HTTP/1.1 200 OK");

          client.println("Content-Type: text/html");

          client.println("Connection: close");  // the connection will be closed after completion of the response

          client.println("Refresh: 60");  // refresh the page automatically every 60 sec

          client.println();

          client.println("<!DOCTYPE HTML>");

          client.println("<html>");
          client.println("GPS Active: ");
          client.println(system.isGpsActive());
          client.println("<br>");

          if (system.isGpsActive()) {
              client.println("Lattitude: ");
              client.println(system.getGpsLat());
              client.println("<br>");
              client.println("Longitude: ");
              client.println(system.getGpsLong());
              client.println("<br>");
          }

          client.println("<br><br><br>");
          client.println("Memory Log:");
          client.println("<br>");
          std::list<std::shared_ptr<String>> memoryLog = system.getLogger().getMemory();
          for(auto it = memoryLog.begin(); it != memoryLog.end(); ++it) {
            client.println(**it);
            client.println("<br>");
          }

          client.println("</html>");

          break;

        }

        if (c == '\n') {

          // starting a new line
          currentLineIsBlank = true;

        } else if (c != '\r') {

          // received a character on the current line
          currentLineIsBlank = false;

        }
      }
    }

    // give the web browser time to receive the data
    delay(1);

    // close the connection:
    client.stop();

    system.getLogger().log(logging::LoggerLevel::LOGGER_LEVEL_INFO, getName(), "Client disconnected");

  }

  return true;
}
