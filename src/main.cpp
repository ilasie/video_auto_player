#include <windows.h>
#include <shellapi.h>
#include <string>
#include <chrono>
#include <sstream>
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

      // Try multiple methods to launch the Movies & TV app
      bool launched = false;
      
      // Using the ms-windows-video URI scheme
      Logger::Log("Trying method 1 (URI scheme)");
      std::string uriCommand = "mswindowsvideo:?action=play&location=" + config.VideoPath;
      HINSTANCE result = ShellExecuteA(NULL, "open", uriCommand.c_str(), NULL, NULL, SW_SHOW);
      
      // According to ShellExecute documentation, values greater than 32 indicate success
      if ((uintptr_t)result > 32) {
        Logger::Log("Method 1 (URI scheme) succeeded");
        launched = true;
      } else {
        std::ostringstream errorMsg;
        errorMsg << "Method 1 (URI scheme) failed with error code: " << (uintptr_t)result;
        Logger::Log(errorMsg.str());
      }
      
      // Method 2: If URI scheme fails, try the AppsFolder approach
      if (!launched) {
        Logger::Log("Trying method 2 (AppsFolder)");
        std::string shellCommand = "shell:AppsFolder\\Microsoft.ZuneVideo_8wekyb3d8bbwe!Microsoft.ZuneVideo";
        result = ShellExecuteA(NULL, "open", shellCommand.c_str(), config.VideoPath.c_str(), NULL, SW_SHOW);
        if ((uintptr_t)result > 32) {
          Logger::Log("Method 2 (AppsFolder) succeeded");
          launched = true;
        } else {
          std::ostringstream errorMsg;
          errorMsg << "Method 2 (AppsFolder) failed with error code: " << (uintptr_t)result;
          Logger::Log(errorMsg.str());
        }
      }
      
      // Method 3: Direct Video.UI.exe execution
      if (!launched) {
        Logger::Log("Trying method 3 (direct execution)");
        SHELLEXECUTEINFOA sei = { 0 };
        sei.cbSize = sizeof(sei);
        sei.fMask = SEE_MASK_NOCLOSEPROCESS;
        sei.lpVerb = "open";
        sei.lpFile = "Video.UI.exe";
        sei.lpParameters = config.VideoPath.c_str();
        sei.nShow = SW_SHOW;

        if (ShellExecuteExA(&sei)) {
          Logger::Log("Method 3 (Video.UI.exe) launched successfully");
          launched = true;
        
          /*  MinGW thread models:
           *  - win32: originate Windows thread, unsupports c++11 std::thread
              - posix: pthreads, supports whole std::thread

              to prevent the compatible problem,
              uses windows.h `Sleep` instead
           */

          // std::this_thread::sleep_for(std::chrono::minutes(1));
          Sleep(60 * 1000);
        }
      }
      
      // If all methods failed
      if (!launched) {
        Logger::Log("Failed to launch any video player after all methods");
      } else {
        // Wait to avoid multiple launches
        Sleep(60 * 1000);
      }
    }

    // std::this_thread::sleep_for(std::chrono::seconds(30));
    Sleep(30 * 1000);
  }

  Logger::Log("Application exited");
  return 0;
}