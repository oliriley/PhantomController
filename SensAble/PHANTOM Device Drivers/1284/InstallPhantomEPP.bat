@echo off
rem Install the Phantom EPP driver using a custom installer
rem %1 location of the driver file (and of DriverInstall.exe)
rem %2 operation, install or uninstall
rem %3 name of the driver file, with extension
rem %4 name of the driver service
rem %5 [test command]
rem 
rem To install PhantomEPP on a 32 bit platform:
rem InstallPhantomEPP.bat "C:\Program Files\SensAble\Phantom Haptic Devices\1284" install PhantomEPP.sys PhantomEPP
rem 
rem To install PhantomEPP on a 64 bit platform:
rem InstallPhantomEPP.bat "C:\Program Files\SensAble\Phantom Haptic Devices\1284" install PhantomEPP_amd64.sys PhantomEPP
rem 
pushd .
cd /D %1
@echo %2ing %3 as %4...
IF NOT EXIST .\DriverInstall.exe goto error
rem Install elevated on Vista platforms:
rem Some computers don't have system32 in their path:
set PATH=%PATH%;%windir%\system32;%windir%\system32\dllcache;
VER | findstr /i /C:"6.0." > nul
IF %ERRORLEVEL% EQU 0 goto vista
:xp
.\DriverInstall.exe %2 %3 %4 %5
goto end
:vista
rem To install on Vista, we MUST run with Admin privileges:
elevate .\DriverInstall.exe "%2 %3 %4 %5"
elevate cmd.exe "/C .\SetPhantomConfigDirectory.bat"
goto end
:error
@echo ************************************************************************
@echo *    ERROR installing PhantomEPP.sys:  DriverInstall.exe not found!    *
@echo ************************************************************************
pause
:end
popd
@echo on
