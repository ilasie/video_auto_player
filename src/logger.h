#ifndef LOGGER_H
#define LOGGER_H

#include <string>

class Logger {
public:
  static void Init();
  static void Log(const std::string& message);
};

#endif