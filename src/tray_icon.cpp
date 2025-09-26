#include "logger.h"
#include "tray_icon.h"
#include <shellapi.h>
#include <shlobj.h>
#include <cstring>

#define WM_TRAYICON    (WM_USER + 1)
#define ID_OPEN_FOLDER 1001
#define ID_EXIT        1002

TrayIcon::TrayIcon(HINSTANCE hInst) : m_hInst(hInst) { }

TrayIcon::~TrayIcon() {
  if (m_hwnd) {
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
    WS_OVERLAPPED,
    0, 0, 0, 0,
    NULL, NULL, m_hInst, this
  );
  if (!m_hwnd) return false;

  NOTIFYICONDATAA nid = { 0 };
  nid.cbSize = sizeof(nid);
  nid.hWnd = m_hwnd;
  nid.uID = 1;
  nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
  nid.uCallbackMessage = WM_TRAYICON;
  nid.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  strcpy(nid.szTip, "Video Auto Player");

  return Shell_NotifyIconA(NIM_ADD, &nid) != FALSE;
}

LRESULT CALLBACK TrayIcon::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  TrayIcon* self = (TrayIcon*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
  
  if (msg == WM_TRAYICON) {
    POINT pt;
    GetCursorPos(&pt);
    if (self) self->ShowContextMenu(hwnd, pt);
  } else if (msg == WM_CREATE) {
    CREATESTRUCT* cs = (CREATESTRUCT*)lParam;
    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)cs->lpCreateParams);
  } else if (msg == WM_COMMAND) {
    int cmd = LOWORD(wParam);
    if (self) {
      if (cmd == ID_EXIT) {
        self->m_running = false;
        PostQuitMessage(0);
      } else if (cmd == ID_OPEN_FOLDER) {
        char path[MAX_PATH];
        if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_PROFILE, NULL, 0, path))) {
          std::string folder = std::string(path) + "\\.video_auto_player";
          ShellExecuteA(NULL, "open", folder.c_str(), NULL, NULL, SW_SHOWNORMAL);
        }
      }
    }
  }
  return DefWindowProc(hwnd, msg, wParam, lParam);
}

void TrayIcon::ShowContextMenu(HWND hwnd, POINT pt) {
  HMENU hMenu = CreatePopupMenu();
  AppendMenuA(hMenu, MF_STRING, ID_OPEN_FOLDER, "Open Config Folder");
  AppendMenuA(hMenu, MF_STRING, ID_EXIT, "Exit");

  SetForegroundWindow(hwnd);
  TrackPopupMenu(hMenu, TPM_BOTTOMALIGN | TPM_LEFTALIGN, pt.x, pt.y, 0, hwnd, NULL);
  DestroyMenu(hMenu);
}