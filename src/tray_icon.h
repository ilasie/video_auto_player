#ifndef TRAY_ICON_H
#define TRAY_ICON_H

#include <windows.h>
#include <string>

class TrayIcon {
public:
  TrayIcon(HINSTANCE hInst);
  ~TrayIcon();
  bool Create();
  bool IsRunning() const { return m_running; }
  void Exit() { m_running = false; }

private:
  static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
  void ShowContextMenu(HWND hwnd, POINT pt);

  HINSTANCE m_hInst;
  HWND m_hwnd;
  bool m_running = true;
};

#endif