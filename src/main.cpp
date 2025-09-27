#include <windows.h>
#include <shellapi.h>
#include <string>
#include <chrono>
#include <sstream>
#include <fstream>
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

  MSG msg;
  while (tray.IsRunning()) {
    // handle window's message
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }

    /* MinGW thread models:
     * - win32: originate Windows thread, unsupports c++11 std::thread
     * - posix: pthreads, supports whole std::thread
     * 
     * to prevent the compatible problem,
     * uses windows.h `Sleep` instead
    */

    Sleep(1000);

    auto now = std::chrono::system_clock::now();
    std::time_t now_t = std::chrono::system_clock::to_time_t(now);
    std::tm local_tm = *std::localtime(&now_t);
    char time_str[6];
    std::strftime(time_str, sizeof(time_str), "%H:%M", &local_tm);
    std::string current_time(time_str);

    Config config = ConfigManager::Load();
    if (config.IsValid() && current_time == config.Time) {
      Logger::Log("Time matched. Launching video: " + config.VideoPath);

      // check if the file exists
      std::ifstream file(config.VideoPath);
      if (!file.good()) {
        Logger::Log("Error: Video file does not exist or is not accessible: " + config.VideoPath);
      } else {
        file.close();
        
        // let system choose default video player
        HINSTANCE result = ShellExecuteA(NULL, "open", config.VideoPath.c_str(), NULL, NULL, SW_SHOW);
        
        // according to ShellExecute documentation, values greater than 32 indicate success
        if ((uintptr_t)result > 32) {
          Logger::Log("Video file opened successfully. Exiting application.");
          tray.Exit();
          break;
        } else {
          std::ostringstream errorMsg;
          errorMsg << "Error opening video file with error code: " << (uintptr_t)result;
          Logger::Log(errorMsg.str());
          
          Logger::Log("Attempting to open file with full path: " + config.VideoPath);
        }
      }
      
      // Wait to avoid multiple launches
      Sleep(60 * 1000);
    }
  }

  Logger::Log("Application exited");
  return (int)msg.wParam;
}