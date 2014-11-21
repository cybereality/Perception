@REM This isn't meant to be run by a double click, if it is, then it will start the installer app

if "%1" EQU "" (
	start VireioDLLInstaller
	goto :EOF
	)

set CURDIR=%CD%

mklink %1\d3d9.dll "%CURDIR%"\d3d9.dll
mklink %1\hijackdll.dll "%CURDIR%"\hijackdll.dll
mklink %1\VRBoost.dll "%CURDIR%"\VRBoost.dll
mklink %1\libfreespace.dll "%CURDIR%"\libfreespace.dll

