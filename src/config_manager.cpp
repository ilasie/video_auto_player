#include "config_manager.h"
#include "logger.h"
#include <fstream>
#include <shlobj.h>
#include "json.hpp" // nlohmann/json

using json = nlohmann::json;

Config ConfigManager::Load() {
  char path[MAX_PATH];
  if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_PROFILE, NULL, 0, path))) {
    std::string configPath = std::string(path) + "\\.video_auto_player\\config.json";
    try {
      std::ifstream f(configPath);
      if (!f.is_open()) {
        Logger::Log("Config file not found: " + configPath);
        return Config {};
      }
      json data = json::parse(f);
      return Config{
        data.value("Video", ""),
        data.value("Time", "")
      };
    } catch (const std::exception& e) {
      Logger::Log("Error parsing config: " + std::string(e.what()));
      return Config {};
    }
  }
  return Config {};
}