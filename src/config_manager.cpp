#include "config_manager.h"
#include "logger.h"
#include <fstream>
#include <shlobj.h>
#include <sys/stat.h>
#include "json.hpp" // nlohmann/json

using json = nlohmann::json;

std::string ConfigManager::GetConfigPath() {
  char path[MAX_PATH];
  if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_PROFILE, NULL, 0, path))) {
    std::string configDir = std::string(path) + "\\.video_auto_player";
    
    // create directory if it doesn't exist
    struct stat info;
    if (stat(configDir.c_str(), &info) != 0) {
      CreateDirectoryA(configDir.c_str(), NULL);
    }
    
    return configDir + "\\config.json";
  }
  return "";
}

void ConfigManager::CreateDefaultConfig() {
  std::string configPath = GetConfigPath();
  if (configPath.empty()) {
    Logger::Log("Failed to get config path");
    return;
  }

  // check if config file already exists
  std::ifstream f(configPath);
  if (f.is_open()) {
    // config file already exists
    f.close();
    return;
  }

  // create default config
  json defaultConfig = {
    {"Video", "C:\\Users\\[安装用户]\\.video_auto_player\\example-exrcise.mp4"},
    {"Time", "09:10"}
  };

  std::ofstream outFile(configPath);
  if (outFile.is_open()) {
    outFile << defaultConfig.dump(4);
    outFile.close();
    Logger::Log("Default config file created at: " + configPath);
  } else {
    Logger::Log("Failed to create default config file at: " + configPath);
  }
}

Config ConfigManager::Load() {
  std::string configPath = GetConfigPath();
  if (configPath.empty()) {
    return Config{};
  }
  
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
  
  return Config {};
}