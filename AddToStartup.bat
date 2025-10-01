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

echo Creating shortcut to "Video Auto Player.exe" in startup dir ...
powershell -Command "$WshShell = New-Object -comObject WScript.Shell; $Shortcut = $WshShell.CreateShortcut('%STARTUP_DIR%\Video Auto Player.lnk'); $Shortcut.TargetPath = '%cd%\Video Auto Player.exe'; $Shortcut.Save()"

if %ERRORLEVEL% EQU 0 (
  echo Succeed in creating shortcut for "Video Auto Player.exe"
  echo Shortcut position: %STARTUP_DIR%\Video Auto Player.lnk
) else (
  echo Error: failed to create shortcut
  pause
  exit /b 1
)

pause