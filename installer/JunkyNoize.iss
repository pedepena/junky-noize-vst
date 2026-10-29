#define PluginName "Junky Noize"
#ifndef PluginVersion
  #define PluginVersion "0.1.0"
#endif

[Setup]
AppId={{0C69A433-C568-4AEC-9A5D-6EC1B12D974A}
AppName={#PluginName}
AppVersion={#PluginVersion}
AppPublisher=Junky Noize
DefaultDirName={commoncf64}\VST3
DisableDirPage=no
DisableProgramGroupPage=yes
DisableWelcomePage=yes
UsePreviousAppDir=no
AppendDefaultDirName=no
DirExistsWarning=no
Uninstallable=yes
OutputDir=output
OutputBaseFilename=JunkyNoize-Setup
ArchitecturesInstallIn64BitMode=x64
ArchitecturesAllowed=x64
PrivilegesRequired=admin
Compression=lzma2
SolidCompression=yes
WizardStyle=modern
CloseApplications=force
RestartApplications=no

[Files]
Source: "..\build\JunkyNoize_artefacts\Release\VST3\Junky Noize.vst3\*"; DestDir: "{app}\Junky Noize.vst3"; Flags: recursesubdirs createallsubdirs ignoreversion
Source: "installer_banner.bmp"; Flags: dontcopy

[UninstallDelete]
Type: filesandordirs; Name: "{app}\Junky Noize.vst3"

[Code]
var
  WelcomeBannerPage: TWizardPage;
  WelcomeBanner: TBitmapImage;

procedure InitializeWizard();
begin
  ExtractTemporaryFile('installer_banner.bmp');
  WelcomeBannerPage := CreateCustomPage(wpWelcome, 'Junky Noize', 'Instala el efecto VST3 en tu sistema.');
  WelcomeBanner := TBitmapImage.Create(WizardForm);
  WelcomeBanner.Parent := WelcomeBannerPage.Surface;
  WelcomeBanner.Left := 0;
  WelcomeBanner.Top := 0;
  WelcomeBanner.Width := WelcomeBannerPage.SurfaceWidth;
  WelcomeBanner.Height := (WelcomeBanner.Width * 600) div 1600;
  WelcomeBanner.Stretch := True;
  WelcomeBanner.Bitmap.LoadFromFile(ExpandConstant('{tmp}\installer_banner.bmp'));
end;
