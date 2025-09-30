@echo off

if not exist "Video Auto Player.exe" (
  echo Error: cannot find "Video Auto Player.exe"
  echo Current dir: %cd%
  pause
  exit /b 1
)

set "STARTUP_DIR=%APPDATA%\Microsoft\Windows\Start Menu\Programs\Startup"

if not exist "%STARTUP_DIR%" (
  echo Error: startup dir does not exist: %STARTUP_DIR%
  pause
  exit /b 1
)

echo Moving "Video Auto Player.exe" to startup dir ...
copy /Y "Video Auto Player.exe" "%STARTUP_DIR%\Video Auto Player.exe"

if %ERRORLEVEL% EQU 0 (
  echo Succeed in copying "Video Auto Player.exe"
  echo Position: %STARTUP_DIR%\Video Auto Player.exe
) else (
  echo Error: failed copied
  pause
  exit /b 1
)

pause