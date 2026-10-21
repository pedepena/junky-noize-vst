#define PluginName "Junky Noize"
#ifndef PluginVersion
  #define PluginVersion "0.1.0"
#endif

[Setup]
AppId={{0C69A433-C568-4AEC-9A5D-6EC1B12D974A}
AppName={#PluginName}
AppVersion={#PluginVersion}
AppPublisher=Junky Noize
DefaultDirName={commoncf}\VST3\Junky Noize.vst3
DisableDirPage=yes
DisableProgramGroupPage=yes
Uninstallable=yes
OutputDir=Output
OutputBaseFilename=JunkyNoize-Setup-{#PluginVersion}-Windows-x64
ArchitecturesInstallIn64BitMode=x64
ArchitecturesAllowed=x64
PrivilegesRequired=admin
Compression=lzma2
SolidCompression=yes
WizardStyle=modern
CloseApplications=force
RestartApplications=no

[Files]
Source: "..\..\build\JunkyNoize_artefacts\Release\VST3\Junky Noize.vst3\*"; DestDir: "{app}"; Flags: recursesubdirs createallsubdirs ignoreversion

[UninstallDelete]
Type: filesandordirs; Name: "{app}"
