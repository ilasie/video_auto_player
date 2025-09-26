#include "logger.h"
#include <fstream>
#include <shlobj.h>
#include <chrono>
#include <iomanip>
#include <sstream>

std::string GetLogPath() {
  char path[MAX_PATH];
  if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_PROFILE, NULL, 0, path))) {
    return std::string(path) + "\\.video_auto_player\\running.log";
  }
  return "running.log";
}

void Logger::Init() {
  char profile[MAX_PATH];
  if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_PROFILE, NULL, 0, profile))) {
    std::string dir = std::string(profile) + "\\.video_auto_player";
    CreateDirectoryA(dir.c_str(), NULL);
  }
}

void Logger::Log(const std::string& message) {
  auto now = std::chrono::system_clock::now();
  std::time_t now_t = std::chrono::system_clock::to_time_t(now);
  std::tm local_tm = *std::localtime(&now_t);
  std::ostringstream oss;
  oss << std::put_time(&local_tm, "%Y-%m-%d %H:%M:%S") << " - " << message << std::endl;

  std::ofstream log(GetLogPath(), std::ios::app);
  if (log.is_open()) {
    log << oss.str();
    log.close();
  }
}