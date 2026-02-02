@echo off
setlocal EnableDelayedExpansion

REM ============================================================================
REM MakeRelease.bat - Build Toolfish release with Visual Studio 2026
REM ============================================================================

set C_TARGET_EXE=app\toolfish.exe
set C_TARGET_DLL=app\sm.dll
set SOLUTION=Toolfish.sln
set CONFIG=Release
set PLATFORM=Win32

REM ============================================================================
REM Pre-flight check: Ensure target files are not locked
REM ============================================================================
echo Checking if target files are accessible...

if exist %C_TARGET_EXE% (
    del /f %C_TARGET_EXE% 2>nul
    if exist %C_TARGET_EXE% (
        echo ERROR: Cannot delete %C_TARGET_EXE% - file is locked!
        echo        Please close Toolfish if it is running.
        goto :error
    )
)

if exist %C_TARGET_DLL% (
    del /f %C_TARGET_DLL% 2>nul
    if exist %C_TARGET_DLL% (
        echo ERROR: Cannot delete %C_TARGET_DLL% - file is locked!
        echo        Please close any program using sm.dll.
        goto :error
    )
)

echo Target files are accessible.

REM ============================================================================
REM Find Visual Studio 2026 installation using vswhere
REM ============================================================================
echo Locating Visual Studio 2026...

set VSWHERE="%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if not exist %VSWHERE% (
    echo ERROR: vswhere.exe not found. Is Visual Studio installed?
    goto :error
)

REM Find VS 2026 (version 18.x) - fall back to latest if not found
for /f "usebackq tokens=*" %%i in (`%VSWHERE% -version [18.0^,19.0^) -latest -requires Microsoft.Component.MSBuild -find MSBuild\**\Bin\MSBuild.exe`) do (
    set MSBUILD=%%i
)

REM If VS 2026 not found, try latest available VS
if not defined MSBUILD (
    echo VS 2026 not found, trying latest Visual Studio...
    for /f "usebackq tokens=*" %%i in (`%VSWHERE% -latest -requires Microsoft.Component.MSBuild -find MSBuild\**\Bin\MSBuild.exe`) do (
        set MSBUILD=%%i
    )
)

if not defined MSBUILD (
    echo ERROR: MSBuild not found. Please install Visual Studio with C++ build tools.
    goto :error
)

echo Found MSBuild: %MSBUILD%

REM ============================================================================
REM Build the solution
REM ============================================================================
echo.
echo Building %SOLUTION% [%CONFIG%^|%PLATFORM%]...
echo.

"%MSBUILD%" %SOLUTION% /p:Configuration=%CONFIG% /p:Platform=%PLATFORM% /m /verbosity:minimal
if errorlevel 1 (
    echo ERROR: Build failed!
    goto :error
)

REM Verify build outputs exist
if not exist %C_TARGET_EXE% (
    echo ERROR: %C_TARGET_EXE% was not created!
    goto :error
)
if not exist %C_TARGET_DLL% (
    echo ERROR: %C_TARGET_DLL% was not created!
    goto :error
)

echo Build successful!

REM ============================================================================
REM Extract version information from source code
REM ============================================================================
echo.
echo Extracting version information from source code...

REM The /u converts decimal point to underscore, /r removes it
util\ctoenv.exe source\CGlobals.h "const float C_F_VERSION = " C_VERSION /u /r
if errorlevel 1 goto :error
call setenv.bat
del setenv.bat

util\ctoenv.exe source\CGlobals.h "const float C_F_VERSION = " C_TEXT_VERSION
if errorlevel 1 goto :error
call setenv.bat
del setenv.bat

echo Version: %C_TEXT_VERSION%

REM ============================================================================
REM Prepare for installer
REM ============================================================================
set C_FILENAME=ToolfishFullInstaller.exe
echo.
echo Building installer: %C_FILENAME%

REM Copy official version of events
copy "SethsEvents\macro - ip address.dat" app\events >nul

set C_README_FILE=readme_full.txt

REM ============================================================================
REM Sign executables
REM ============================================================================
echo.
echo Signing executables...

copy app\Toolfish.exe ..\Signing >nul
if errorlevel 1 (
    echo ERROR: Failed to copy Toolfish.exe to Signing folder!
    goto :error
)

copy app\sm.dll ..\Signing >nul
if errorlevel 1 (
    echo ERROR: Failed to copy sm.dll to Signing folder!
    goto :error
)

cd ..\Signing
call sign.bat Toolfish.exe "Toolfish" "www.rtsoft.com"
if errorlevel 1 (
    cd ..\Toolfish
    echo ERROR: Failed to sign Toolfish.exe!
    goto :error
)

call sign.bat sm.dll "Toolfish" "www.rtsoft.com"
if errorlevel 1 (
    cd ..\Toolfish
    echo ERROR: Failed to sign sm.dll!
    goto :error
)
cd ..\Toolfish

echo Copying signed files back to app folder...
copy /Y ..\Signing\Toolfish.exe app >nul
if errorlevel 1 (
    echo ERROR: Failed to copy signed Toolfish.exe back to app folder!
    echo        The file may be locked by another process.
    goto :error
)

copy /Y ..\Signing\sm.dll app >nul
if errorlevel 1 (
    echo ERROR: Failed to copy signed sm.dll back to app folder!
    echo        The file may be locked by another process.
    goto :error
)

echo Executables signed successfully.

REM ============================================================================
REM Build installer with NSIS
REM ============================================================================
echo.
echo Building NSIS installer...

cd win
..\..\util\NSIS\makensis.exe toolfish.nsi
if errorlevel 1 (
    cd ..
    echo ERROR: NSIS installer build failed!
    goto :error
)
cd ..

REM ============================================================================
REM Sign the installer
REM ============================================================================
echo.
echo Signing installer...

if not exist dist\ToolfishFullInstaller.exe (
    echo ERROR: dist\ToolfishFullInstaller.exe was not created by NSIS!
    goto :error
)

copy dist\ToolfishFullInstaller.exe ..\Signing >nul
if errorlevel 1 (
    echo ERROR: Failed to copy installer to Signing folder!
    goto :error
)

cd ..\Signing
call sign.bat ToolfishFullInstaller.exe "Toolfish" "www.rtsoft.com"
if errorlevel 1 (
    cd ..\Toolfish
    echo ERROR: Failed to sign ToolfishFullInstaller.exe!
    goto :error
)

copy /Y ToolfishFullInstaller.exe ..\Toolfish\dist >nul
if errorlevel 1 (
    cd ..\Toolfish
    echo ERROR: Failed to copy signed installer back to dist folder!
    goto :error
)
cd ..\Toolfish

echo Installer signed successfully.

REM ============================================================================
REM Cleanup and finish
REM ============================================================================
set C_VERSION=
set C_TEXT_VERSION=
set C_TARGET_EXE=
set C_TARGET_DLL=

echo.
echo ============================================================================
echo Build successfully completed!
echo Output: dist\%C_FILENAME%
echo ============================================================================
pause
goto :eof

:error
echo.
echo ============================================================================
echo BUILD FAILED
echo ============================================================================
util\beeper.exe /p
pause
exit /b 1
