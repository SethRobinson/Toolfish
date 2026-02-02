;NSIS Modern User Interface version 1.70
;Header Bitmap Example Script
;Written by Joost Verburg

;--------------------------------
;Include Modern UI

  !include "MUI.nsh"
 !include "LogicLib.nsh"
;--------------------------------
;General
  SetCompressor lzma
  !define _TITLE_ "Toolfish Utility Suite"
  !define _VERSION_ "$%C_TEXT_VERSION%"
  !define _COMPILE_DATE_ "${__DATE__}"
  !define _README_FILE_ "$%C_README_FILE%"

  ;Name and file
  Name "${_TITLE_}"
 ; Icon
!define MUI_ICON "toolfish.ico" 
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\classic-uninstall.ico"

RequestExecutionLevel user

OutFile "..\dist\$%C_FILENAME%"

!define MUI_WELCOMEFINISHPAGE_BITMAP "welcome_side.bmp"
!define MUI_UNWELCOMEFINISHPAGE_BITMAP "welcome_side.bmp"

  ;Default installation folder
  InstallDir "$LOCALAPPDATA\Toolfish"
  
  ;Get installation folder from registry if available
  InstallDirRegKey HKCU "Software\RTSOFT\TOOLFISH" "path"

BrandingText " "
;InitPluginsDir
;  File /oname=$PLUGINSDIR\splash.bmp "path\to\your\bitmap.bmp"
;  advsplash::show 1000 600 400 -1 $PLUGINSDIR\splash
;  Pop $0

;--------------------------------
;Interface Configuration

  !define MUI_HEADERIMAGE
  !define MUI_HEADERIMAGE_BITMAP "header.bmp"
  !define MUI_HEADERIMAGE_UNBITMAP "header.bmp"
  !define MUI_ABORTWARNING
 
;--------------------------------
;Pages

  
  !define MUI_WELCOMEPAGE_TEXT "This wizard will guide you through the installation of ${_TITLE_}. (${_VERSION_} released on ${_COMPILE_DATE_})\r\n\r\nClick Next to continue."
  !define MUI_WELCOMEPAGE_TITLE "${_TITLE_} ${_VERSION_} Installer (Full version!)"
  !insertmacro MUI_PAGE_WELCOME
  !insertmacro MUI_PAGE_LICENSE "${_README_FILE_}"
  
  
  ;Customize component texts
  
  !define MUI_COMPONENTSPAGE_TEXT_COMPLIST "Select extra components to install."
  !define MUI_COMPONENTSPAGE_TEXT_TOP "Here, you can choose whether or not you want a desktop icon in addition to the normal start menu options."
  !insertmacro MUI_PAGE_COMPONENTS
  
  
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
 
!define MUI_FINISHPAGE_LINK "Click here to visit the RTsoft website"
!define MUI_FINISHPAGE_LINK_LOCATION "http://www.rtsoft.com/"
!define MUI_FINISHPAGE_RUN
!define MUI_FINISHPAGE_RUN_FUNCTION "LaunchLink"
 
 !insertmacro MUI_PAGE_FINISH
  
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
 
;--------------------------------
;Languages
 
  !insertmacro MUI_LANGUAGE "English"

;--------------------------------
;Installer Sections

;Remove the - part in from of Main Game if you don't want it to be hidden...
Section "-Main Game" SecMain
SectionIn RO
  SetOutPath "$INSTDIR"
  File "..\app\toolfish.exe"
  
  IfFileExists $INSTDIR\sm.dll Skip
  File "..\app\sm.dll"
  Skip:
  File "..\app\whatsnew.txt"

 
	File /r "..\app\docs"
	File /r "..\app\gui"
	File /r "..\app\media"

    File "..\app\whatsnew.txt"



CreateDirectory "$INSTDIR\cache"
CreateDirectory "$INSTDIR\events"

SetOutPath "$INSTDIR\events"

  File "..\app\events\check rtsoft.dat"
  File "..\app\events\check toolfish website.dat"
  File "..\app\events\hotkey - dictionary.com.dat"
  File "..\app\events\hotkey - google.dat"
  File "..\app\events\hotkey_stealth.dat"
  File "..\app\events\hotkey_toggle.dat"
  File "..\app\events\macro - ip address.dat"
  File "..\app\events\open notes.dat"
  

CreateDirectory "$SMPROGRAMS\${_TITLE_}"
;ok, this will create a Folder in your Start menue

;startup

  WriteRegStr HKCU "SOFTWARE\Microsoft\Windows\CurrentVersion\Run" "Toolfish" "$INSTDIR\toolfish.exe -minimize"


  ;Store installation folder
  WriteRegStr HKCU "Software\RTSOFT\NOVASHELL" "path" $INSTDIR

  ;write uninstall strings
  WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\${_TITLE_}" "DisplayName" "${_TITLE_} (remove only)"
  WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\${_TITLE_}" "UninstallString" '"$INSTDIR\Uninstall.exe"'
  
  ;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"
  CreateShortCut "$SMPROGRAMS\${_TITLE_}\${_TITLE_}.lnk" "$INSTDIR\toolfish.exe" "" "$INSTDIR\toolfish.exe"
  WriteIniStr "$SMPROGRAMS\${_TITLE_}\Report a bug or make a comment.url" "InternetShortcut" "URL" "http://www.rtsoft.com/pages/feedback_app.htm?game=${_TITLE_}&version=${_VERSION_}"
  CreateShortCut "$SMPROGRAMS\${_TITLE_}\What's new in this version.lnk" "$INSTDIR\whatsnew.txt" ; use defaults for parameters, icon, etc.
  CreateShortCut "$SMPROGRAMS\${_TITLE_}\Help.lnk" "$INSTDIR\docs\help.htm" ; use defaults for parameters, icon, etc.
  WriteIniStr "$SMPROGRAMS\${_TITLE_}\Robinson Technologies Website.url" "InternetShortcut" "URL" "http://www.rtsoft.com"
  CreateShortCut "$SMPROGRAMS\${_TITLE_}\Uninstall ${_TITLE_}.lnk" "$INSTDIR\Uninstall.exe" ; use defaults for parameters, icon, etc.

; file associations

; back up old value of .opt
;!define Index "Line${__LINE__}"
;removed association stuff
;!undef Index

SectionEnd


;Installer Sections

Section "Desktop Icon" SecDesktopIcon
  SetOutPath "$INSTDIR"
  CreateShortCut "$DESKTOP\${_TITLE_}.lnk" "$INSTDIR\toolfish.exe" 

SectionEnd

Function LaunchLink
  ExecShell "" "$INSTDIR\toolfish.exe"
FunctionEnd



;--------------------------------
;Descriptions

  ;Language strings
  LangString DESC_SecMain ${LANG_ENGLISH} "The main game files, these are required to play the game."
  LangString DESC_SecDesktopIcon ${LANG_ENGLISH} "This option will throw one of those handy desktop icons on the main menu for easy access to the program."

  ;Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${SecMain} $(DESC_SecMain)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecDesktopIcon} $(DESC_SecDesktopIcon)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END
 
;--------------------------------
;Uninstaller Section

Section "Uninstall"

;default delete

  Delete "$INSTDIR\toolfish.exe"
  Delete "$INSTDIR\whatsnew.txt"
 
  Delete "$INSTDIR\Uninstall.exe"
  Delete "$INSTDIR\sm.dll"
  
   RMDir /r "$INSTDIR\docs"
   RMDir /r "$INSTDIR\media"
   RMDir /r "$INSTDIR\gui"
 

  DeleteRegValue HKCU "SOFTWARE\Microsoft\Windows\CurrentVersion\Run" "Toolfish"


  DeleteRegKey HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\${_TITLE_}"
  Delete "$SMPROGRAMS\${_TITLE_}\*.*"
  RMDir "$SMPROGRAMS\${_TITLE_}"
  Delete "$DESKTOP\${_TITLE_}.lnk"

;delete user stuff too?

MessageBox MB_YESNO "Would you like to also delete all events, settings and cached information?  (Only say yes if you want to remove every trace of Toolfish from your computer!)" IDNO skip_it 
  RMDir /r "$INSTDIR\cache"
  RMDir /r "$INSTDIR\events"
  Delete "$INSTDIR\toolfish_log.txt"
  Delete "$INSTDIR\config.dat"
  Delete "$INSTDIR\notes.txt"
  DeleteRegKey HKLM "Software\RTSOFT\TOOLFISH\path"
  DeleteRegKey /ifempty HKCU "Software\RTSOFT\TOOLFISH"
 

  RMDir "$INSTDIR" 

 
  ;rest of script 
skip_it:

SectionEnd