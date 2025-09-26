#include <windows.h>
#include <shellapi.h>
#include <string>
#include <chrono>
#include "config_manager.h"
#include "logger.h"
#include "tray_icon.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
  HWND console = GetConsoleWindow();
  if (console) ShowWindow(console, SW_HIDE);

  Logger::Init();

  TrayIcon tray(hInstance);
  if (!tray.Create()) {
    Logger::Log("Failed to create tray icon");
    return 1;
  }

  while (tray.IsRunning()) {
    auto now = std::chrono::system_clock::now();
    std::time_t now_t = std::chrono::system_clock::to_time_t(now);
    std::tm local_tm = *std::localtime(&now_t);
    char time_str[6];
    std::strftime(time_str, sizeof(time_str), "%H:%M", &local_tm);
    std::string current_time(time_str);

    Config config = ConfigManager::Load();
    if (config.IsValid() && current_time == config.Time) {
      Logger::Log("Time matched. Launching video: " + config.VideoPath);

      // First, try to launch with explorer shell command (for UWP apps)
      std::string shellCommand = "shell:AppsFolder\\Microsoft.ZuneVideo_8wekyb3d8bbwe!Microsoft.ZuneVideo";
      HINSTANCE result = ShellExecuteA(NULL, "open", shellCommand.c_str(), config.VideoPath.c_str(), NULL, SW_SHOW);
      
      // If that fails, try direct Video.UI.exe execution
      if ((int)result <= 32) {
        SHELLEXECUTEINFOA sei = { 0 };
        sei.cbSize = sizeof(sei);
        sei.fMask = SEE_MASK_NOCLOSEPROCESS;
        sei.lpVerb = "open";
        sei.lpFile = "Video.UI.exe";
        sei.lpParameters = config.VideoPath.c_str();
        sei.nShow = SW_SHOW;

        if (ShellExecuteExA(&sei)) {
          Logger::Log("Video.UI launched successfully");
          
          /*  MinGW thread models:
           *  - win32: originate Windows thread, unsupports c++11 std::thread
              - posix: pthreads, supports whole std::thread

              to prevent the compatible problem,
              uses windows.h `Sleep` instead
           */

          // std::this_thread::sleep_for(std::chrono::minutes(1));
          Sleep(60 * 1000);
        } else {
          Logger::Log("Failed to launch Video.UI");
        }
      } else {
        Logger::Log("Video launched successfully via AppsFolder");
        Sleep(60 * 1000);
      }
    }

    // std::this_thread::sleep_for(std::chrono::seconds(30));
    Sleep(30 * 1000);
  }

  Logger::Log("Application exited");
  return 0;
}