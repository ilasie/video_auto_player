#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <string>

struct Config {
  std::string VideoPath;
  std::string Time;
  bool IsValid() const { return !VideoPath.empty() && !Time.empty(); }
};

class ConfigManager {
public:
  static Config Load();
  static void CreateDefaultConfig();

private:
  static std::string GetConfigPath();
};

#endif