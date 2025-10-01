; Inno Setup compiling script

[Setup]
AppName=Video Auto Player
AppVersion=1.0
AppPublisher=ilasie
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
Name: "en"; MessagesFile: "compiler:Default.isl"
Name: "zh"; MessagesFile: "compiler:Languages\ChineseSimplified.isl"

[CustomMessages]
en.DesktopIconCheckboxCaption=Create a desktop icon(&D)
zh.DesktopIconCheckboxCaption=创建桌面图标(&D)
en.StartupTaskDescription=Launch on Windows startup
zh.StartupTaskDescription=开机启动
en.StartupTaskGroupDescription=Startup options:
zh.StartupTaskGroupDescription=启动选项:
en.AdditionalIconsCaption=Additional icons
zh.AdditionalIconsCaption=附加图标
en.LaunchProgram=Launch Video Auto Player
zh.LaunchProgram=启动 Video Auto Player

[Tasks]
Name: "desktopicon"; Description: "{cm:DesktopIconCheckboxCaption}"; GroupDescription: "{cm:AdditionalIconsCaption}"; Flags: unchecked
Name: "startup"; Description: "{cm:StartupTaskDescription}"; GroupDescription: "{cm:StartupTaskGroupDescription}"; Flags: unchecked

[Files]
Source: "Video Auto Player.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "README.md"; DestDir: "{app}"; Flags: ignoreversion
source: "NOTICE"; Destdir: "{app}"; Flags: ignoreversion
source: "LICENSE"; DestDir: "{app}"; Flags: ignoreversion
Source: "example.mp4"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{group}\Video Auto Player"; Filename: "{app}\Video Auto Player.exe"
Name: "{group}\README"; Filename: "{app}\README.md"
Name: "{group}\{cm:UninstallProgram,Video Auto Player}"; Filename: "{uninstallexe}"
Name: "{autodesktop}\Video Auto Player"; Filename: "{app}\Video Auto Player.exe"; Tasks: desktopicon
Name: "{userstartup}\Video Auto Player"; Filename: "{app}\Video Auto Player.exe"; Tasks: startup

[Run]
Filename: "{app}\Video Auto Player.exe"; Description: "{cm:LaunchProgram,Video Auto Player}"; Flags: nowait postinstall skipifsilent

[UninstallDelete]
Type: files; Name: "{userdocs}\.video_auto_player\config.json"
Type: files; Name: "{userdocs}\.video_auto_player\example.mp4"
Type: dirifempty; Name: "{userdocs}\.video_auto_player"

[Code]
procedure CurStepChanged(CurStep: TSetupStep);
var
  ConfigDir: String;
begin
  if (CurStep = ssPostInstall) then
  begin
    ConfigDir := ExpandConstant('{userdocs}\.video_auto_player');
    if not DirExists(ConfigDir) then
      CreateDir(ConfigDir);
    
    if FileExists(ExpandConstant('{app}\example.mp4')) then
      CopyFile(ExpandConstant('{app}\example.mp4'), ConfigDir + '\example.mp4', False);
  end;
end;