; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "TaMiSe"
#define MyAppVersion "0.5.5_DevBuild"
#define MyAppPublisher "LE GOUIL Kerwan"
#define MyAppURL "http://tamise.naeo.fr"
#define MyAppExeName "Tamise.exe"

#define MyAppSources "sources"
#define MyAppExec "build-Tamise-Desktop_Qt_5_11_0_MinGW_32bit-Release\release"
#define MyAppBin "bin"
#define MyAppROOT "E:\Projets\Tamise\"
 
#define MyDateTimeString GetDateTimeString('dd/mm/yyyy hh:nn:ss', '-', ':');
#define MyDateTimeLong GetDateTimeString('yyyy_mm_dd_hh_nn_ss', '', '');

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{139EFEAC-F9DB-4ED8-878F-3464AAD3A9F3}
AppName={#MyAppName}
AppVersion={#MyAppVersion} (Build Installateur : {#MyDateTimeString})
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\{#MyAppName}
DefaultGroupName={#MyAppName}
AllowNoIcons=yes
LicenseFile={#MyAppROOT}\sources\res\license.txt
OutputDir={#MyAppROOT}\setup
OutputBaseFilename=setup_{#MyAppName}_{#MyAppVersion}_{#MyDateTimeLong}
SetupIconFile={#MyAppROOT}\sources\res\mixer.ico
Compression=lzma
SolidCompression=yes

ChangesAssociations=yes
 
[Registry]
Root: HKCR; Subkey: ".tms"; ValueType: string; ValueName: ""; ValueData: "{#MyAppName}"; Flags: uninsdeletevalue
Root: HKCR; Subkey: "{#MyAppName}"; ValueType: string; ValueName: ""; ValueData: "My Program File"; Flags: uninsdeletekey
Root: HKCR; Subkey: "{#MyAppName}\DefaultIcon"; ValueType: string; ValueName: ""; ValueData: "{app}\Tamise.exe,0"
Root: HKCR; Subkey: "{#MyAppName}\shell\open\command"; ValueType: string; ValueName: ""; ValueData: """{app}\Tamise.exe"" ""%1"""


[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "french"; MessagesFile: "compiler:Languages\French.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked; OnlyBelowVersion: 0,6.1

[Files]
Source: "{#MyAppROOT}{#MyAppExec}\Tamise.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppROOT}{#MyAppBin}\fmodex.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppROOT}{#MyAppBin}\libgcc_s_dw2-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppROOT}{#MyAppBin}\libstdc++-6.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppROOT}{#MyAppBin}\libwinpthread-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppROOT}{#MyAppBin}\Qt5Core.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppROOT}{#MyAppBin}\Qt5Gui.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppROOT}{#MyAppBin}\Qt5Network.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppROOT}{#MyAppBin}\Qt5Widgets.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppROOT}{#MyAppBin}\Qt5Xml.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppROOT}{#MyAppBin}\platforms\qwindows.dll"; DestDir: "{app}\platforms"; Flags: ignoreversion
Source: "{#MyAppROOT}\sources.7z"; DestDir: "{app}"; Flags: ignoreversion
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: quicklaunchicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent
