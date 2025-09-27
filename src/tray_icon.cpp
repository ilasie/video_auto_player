#include "logger.h"
#include "tray_icon.h"
#include <shellapi.h>
#include <shlobj.h>
#include <windowsx.h>
#include <cstring>

#define WM_TRAYICON    (WM_USER + 1)
#define ID_OPEN_FOLDER 1001
#define ID_EXIT        1002
#define ID_TIMER       1003

TrayIcon::TrayIcon(HINSTANCE hInst) : m_hInst(hInst), m_hwnd(NULL), m_running(true) { }

TrayIcon::~TrayIcon() {
  if (m_hwnd) {
    KillTimer(m_hwnd, ID_TIMER);
    NOTIFYICONDATAA nid = { 0 };
    nid.cbSize = sizeof(nid);
    nid.hWnd = m_hwnd;
    nid.uID = 1;
    Shell_NotifyIconA(NIM_DELETE, &nid);
    DestroyWindow(m_hwnd);
  }
}

bool TrayIcon::Create() {
  WNDCLASSEXA wc = { 0 };
  wc.cbSize = sizeof(wc);
  wc.lpfnWndProc = WndProc;
  wc.hInstance = m_hInst;
  wc.lpszClassName = "VideoAutoPlayerTray";
  RegisterClassExA(&wc);

  m_hwnd = CreateWindowA(
    wc.lpszClassName,
    "",
    WS_POPUP,
    0, 0, 0, 0,
    NULL, NULL, m_hInst, this
  );
  
  if (!m_hwnd) {
    Logger::Log("Failed to create window for tray icon");
    return false;
  }

  Logger::Log("Created window with hwnd: " + std::to_string((uintptr_t)m_hwnd));
  
  // 确保在创建窗口后设置用户数据
  SetWindowLongPtr(m_hwnd, GWLP_USERDATA, (LONG_PTR)this);
  Logger::Log("Set window user data immediately after window creation");
  
  // 确保窗口可见
  ShowWindow(m_hwnd, SW_HIDE);
  
  // 设置定时器，定期检查托盘图标状态
  SetTimer(m_hwnd, ID_TIMER, 5000, NULL);
  Logger::Log("Timer set with ID: " + std::to_string(ID_TIMER));

  NOTIFYICONDATAA nid = { 0 };
  nid.cbSize = sizeof(nid);
  nid.hWnd = m_hwnd;
  nid.uID = 1;
  nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
  nid.uCallbackMessage = WM_TRAYICON;
  nid.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  strcpy(nid.szTip, "Video Auto Player");

  Logger::Log("Adding tray icon with hwnd: " + std::to_string((uintptr_t)nid.hWnd) + ", uID: " + std::to_string(nid.uID) + ", callback message: 0x" + std::to_string(nid.uCallbackMessage));
  
  BOOL result = Shell_NotifyIconA(NIM_ADD, &nid);
  Logger::Log("Shell_NotifyIconA result: " + std::to_string(result));
  
  if (!result) {
    Logger::Log("Failed to add tray icon");
  }
  
  return result != FALSE;
}

LRESULT CALLBACK TrayIcon::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  TrayIcon* self = (TrayIcon*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
  
  // 只记录关键消息，避免日志过多影响性能
  if (msg == WM_TRAYICON || msg == WM_CREATE || msg == WM_COMMAND || msg == WM_DESTROY || msg == WM_TIMER) {
    std::string msgStr = "WndProc received message: 0x";
    char msgBuffer[16];
    sprintf_s(msgBuffer, "%X", msg);
    msgStr += std::string(msgBuffer);
    msgStr += " (hwnd: " + std::to_string((uintptr_t)hwnd) + ", wParam: " + std::to_string(wParam) + ", lParam: " + std::to_string(lParam) + ")";
    Logger::Log(msgStr);
  }
  
  if (msg == WM_TRAYICON) {
    // 不再记录每个WM_TRAYICON消息的详细信息，只记录关键事件
    if (lParam == WM_RBUTTONUP) {
      Logger::Log("Processing RBUTTONUP");
      // 使用 GetMessagePos() 获取准确的鼠标位置，解决高DPI环境下的菜单位置偏移问题
      DWORD msgPos = GetMessagePos();
      POINT pt;
      pt.x = (short)LOWORD(msgPos);
      pt.y = (short)HIWORD(msgPos);
      
      // 确保坐标有效
      if (pt.x >= 0 && pt.y >= 0) {
        Logger::Log("Mouse position: " + std::to_string(pt.x) + ", " + std::to_string(pt.y));
        if (self) {
          Logger::Log("Calling ShowContextMenu");
          self->ShowContextMenu(hwnd, pt);
        } else {
          Logger::Log("Error: 'self' pointer is null in WndProc.");
        }
      }
    } else if (lParam == WM_LBUTTONUP) {
      Logger::Log("Processing LBUTTONUP");
      // Handle left button click - for example, open config folder
      char path[MAX_PATH];
      if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_PROFILE, NULL, 0, path))) {
        std::string folder = std::string(path) + "\\.video_auto_player";
        ShellExecuteA(NULL, "open", folder.c_str(), NULL, NULL, SW_SHOWNORMAL);
      }
    } else if (lParam == WM_LBUTTONDBLCLK) {
      Logger::Log("Processing LBUTTONDBLCLK");
    } else if (lParam == WM_RBUTTONDBLCLK) {
      Logger::Log("Processing RBUTTONDBLCLK");
    } else if (lParam == WM_MBUTTONUP) {
      Logger::Log("Processing MBUTTONUP");
    } else if (lParam == WM_MBUTTONDBLCLK) {
      Logger::Log("Processing MBUTTONDBLCLK");
    }
    return 0; // 处理完WM_TRAYICON消息后直接返回
  } else if (msg == WM_CREATE) {
    Logger::Log("Processing WM_CREATE message with wParam: " + std::to_string(wParam) + ", lParam: " + std::to_string(lParam));
    CREATESTRUCT* cs = (CREATESTRUCT*)lParam;
    if (cs) {
      SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)cs->lpCreateParams);
      Logger::Log("Set window user data from CREATESTRUCT");
    }
  } else if (msg == WM_COMMAND) {
    Logger::Log("Processing WM_COMMAND message with wParam: " + std::to_string(wParam));
    int cmd = LOWORD(wParam);
    if (self) {
      if (cmd == ID_EXIT) {
        Logger::Log("Exit command received");
        self->m_running = false;
        PostQuitMessage(0);
      } else if (cmd == ID_OPEN_FOLDER) {
        Logger::Log("Open folder command received");
        char path[MAX_PATH];
        if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_PROFILE, NULL, 0, path))) {
          std::string folder = std::string(path) + "\\.video_auto_player";
          ShellExecuteA(NULL, "open", folder.c_str(), NULL, NULL, SW_SHOWNORMAL);
        }
      }
    } else {
      Logger::Log("Error: 'self' pointer is null in WM_COMMAND.");
    }
  } else if (msg == WM_TIMER) {
    if (wParam == ID_TIMER) {
      Logger::Log("Timer tick: Checking tray icon status");
      // 可以在这里添加检查托盘图标状态的代码
    }
  } else if (msg == WM_DESTROY) {
    Logger::Log("Processing WM_DESTROY message");
    PostQuitMessage(0);
  }
  return DefWindowProc(hwnd, msg, wParam, lParam);
}

void TrayIcon::ShowContextMenu(HWND hwnd, POINT pt) {
  Logger::Log("ShowContextMenu called with hwnd: " + std::to_string((uintptr_t)hwnd) + 
              " at position: (" + std::to_string(pt.x) + ", " + std::to_string(pt.y) + ")");
  
  HMENU hMenu = CreatePopupMenu();
  if (!hMenu) {
    Logger::Log("Failed to create popup menu");
    return;
  }
  
  Logger::Log("Popup menu created successfully");
  AppendMenuA(hMenu, MF_STRING, ID_OPEN_FOLDER, "Open Config Folder");
  AppendMenuA(hMenu, MF_STRING, ID_EXIT, "Exit");
  
  Logger::Log("Menu items added");

  SetForegroundWindow(hwnd);
  Logger::Log("Set foreground window");
  
  Logger::Log("About to call TrackPopupMenu at position: " + std::to_string(pt.x) + ", " + std::to_string(pt.y));
  
  // 使用TPM_RIGHTBUTTON标志确保右键点击时正确显示菜单
  UINT uFlags = TPM_RIGHTBUTTON | TPM_LEFTALIGN | TPM_BOTTOMALIGN;
  TrackPopupMenu(hMenu, uFlags, pt.x, pt.y, 0, hwnd, NULL);
  
  Logger::Log("TrackPopupMenu called");
  
  // 发送空消息以避免菜单显示后窗口失去焦点的问题
  PostMessage(hwnd, WM_NULL, 0, 0);
  
  DestroyMenu(hMenu);
  Logger::Log("ShowContextMenu finished");
}