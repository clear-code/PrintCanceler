;PrintCanceler Setup--

[Setup]
AppName=PrintCanceler
AppVerName=PrintCanceler
VersionInfoVersion=1.0.0.0
AppVersion=1.0.0.0
AppMutex=PrintCancelerSetup
;DefaultDirName=C:\PrintCanceler
DefaultDirName={code:GetProgramFiles}\PrintCanceler
Compression=lzma2
SolidCompression=yes
OutputDir=SetupOutput
OutputBaseFilename=PrintCancelerSetup_x86
AppPublisher=PrintCanceler
WizardImageStretch=no
VersionInfoDescription=PrintCancelerSetup
ArchitecturesAllowed=x86
DefaultGroupName=PrintCanceler
UninstallDisplayIcon={app}\PrintCanceler.exe

[Registry]
Root: HKLM; Subkey: "Software\PrintCanceler"; Flags: uninsdeletekey
Root: HKLM; Subkey: "Software\PrintCanceler"; ValueType: string; ValueName: "Path"; ValueData: "{app}\"
Root: HKLM; Subkey: "Software\PrintCanceler"; ValueType: string; ValueName: "ClientType"; ValueData: ""
Root: HKLM; Subkey: "Software\PrintCanceler"; ValueType: string; ValueName: "Version"; ValueData: "1.0.0.0"
;Root: HKLM; Subkey: "Software\PrintCanceler"; ValueType: string; ValueName: "Rulefile"; ValueData: "{app}\PrintCanceler.ini"
;Root: HKLM; Subkey: "Software\PrintCanceler"; ValueType: string; ValueName: "RCAPfile"; ValueData: "{app}\ResourceCap.ini"
Root: HKLM; Subkey: "Software\PrintCanceler"; ValueType: string; ValueName: "ExtensionExecfile"; ValueData: "{app}\PrintCanceler.exe"

;Edge
Root: HKLM; Subkey: "SOFTWARE\Microsoft\Edge\NativeMessagingHosts\com.clear_code.repost_confirmation_canceler"; Flags: uninsdeletekey
Root: HKLM; Subkey: "SOFTWARE\Microsoft\Edge\NativeMessagingHosts\com.clear_code.repost_confirmation_canceler"; ValueType: string; ValueData: "{app}\PrintCancelerHost\edge.json";

[Languages]
Name: jp; MessagesFile: "compiler:Languages\Japanese.isl"


[Files]
;exe
Source: "bin\Release\PrintCanceler.exe"; DestDir: "{app}\";Flags: ignoreversion;permissions:users-readexec admins-full system-full
;ini
;Source: "Resources\PrintCanceler.ini"; DestDir: "{app}"; Flags: onlyifdoesntexist

;host
Source: "bin\Release\PrintCancelerTalk.exe"; DestDir: "{app}\PrintCancelerHost";Flags: ignoreversion;permissions:users-readexec admins-full system-full

;edge
Source: "Resources\edge.json"; DestDir: "{app}\PrintCancelerHost";Flags: ignoreversion;permissions:users-readexec admins-full system-full

[Dirs]
Name: "{app}";Permissions: users-modify

[Run] 
Filename: "{sys}\icacls.exe";Parameters: """{app}\PrintCanceler.exe"" /inheritance:r"; Flags: runhidden shellexec
Filename: "{sys}\icacls.exe";Parameters: """{app}\PrintCancelerHost\PrintCancelerTalk.exe"" /inheritance:r"; Flags: runhidden shellexec
Filename: "{sys}\icacls.exe";Parameters: """{app}\PrintCancelerHost\edge.json"" /inheritance:r"; Flags: runhidden shellexec

[UninstallRun]

[Code]
function GetProgramFiles(Param: string): string;
  begin
    if IsWin64 then Result := ExpandConstant('{pf64}')
    else Result := ExpandConstant('{pf32}')
  end;

procedure TaskKill(FileName: String);
var
  ResultCode: Integer;
begin
    Exec(ExpandConstant('taskkill.exe'), '/f /im ' + '"' + FileName + '"', '', SW_HIDE,ewWaitUntilTerminated, ResultCode);
end;
function InitializeSetup():Boolean;
begin 
	TaskKill('msedge.exe');
	TaskKill('PrintCanceler.exe');
	Result := True; 
end; 
