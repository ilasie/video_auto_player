; Inno Setup 脚本文件
; 用于创建 Video Auto Player 安装程序

[Setup]
AppName=Video Auto Player
AppVersion=1.1
AppPublisher=Video Auto Player Team
AppPublisherURL=https://github.com/ilasie/video_auto_player
AppSupportURL=https://github.com/ilasie/video_auto_player
AppUpdatesURL=https://github.com/ilasie/video_auto_player/releases
DefaultDirName={autopf}\Video Auto Player
DefaultGroupName=Video Auto Player
OutputDir=.
OutputBaseFilename=video_auto_player_installer
Compression=lzma
SolidCompression=yes
WizardStyle=modern
PrivilegesRequired=admin

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "chinese"; MessagesFile: "compiler:Languages\ChineseSimplified.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "startup"; Description: "开机启动"; GroupDescription: "启动选项:"; Flags: unchecked

[Files]
Source: "Video Auto Player.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "AddToStartup.bat"; DestDir: "{app}"; Flags: ignoreversion
Source: "README.md"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{group}\Video Auto Player"; Filename: "{app}\Video Auto Player.exe"
Name: "{group}\README"; Filename: "{app}\README.md"
Name: "{group}\{cm:UninstallProgram,Video Auto Player}"; Filename: "{uninstallexe}"
Name: "{autodesktop}\Video Auto Player"; Filename: "{app}\Video Auto Player.exe"; Tasks: desktopicon

[Run]
Filename: "{app}\Video Auto Player.exe"; Description: "{cm:LaunchProgram,Video Auto Player}"; Flags: nowait postinstall skipifsilent
Filename: "{app}\AddToStartup.bat"; Description: "添加到开机启动"; Flags: runhidden; Tasks: startup

[UninstallDelete]
Type: files; Name: "{app}\config.json"

[Code]
procedure CurStepChanged(CurStep: TSetupStep);
begin
  if (CurStep = ssPostInstall) then
  begin
    // 安装后创建默认配置文件目录
    if not DirExists(ExpandConstant('{userdocs}\.video_auto_player')) then
      CreateDir(ExpandConstant('{userdocs}\.video_auto_player'));
  end;
end;