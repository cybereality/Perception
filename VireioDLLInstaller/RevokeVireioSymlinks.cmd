@REM This isn't meant to be run by a double click, if it is, then it will start the installer app

if "%1" EQU "" (
	start VireioDLLInstaller
	goto :EOF
	)

del %1\d3d9.dll
del %1\hijackdll.dll
del %1\VRBoost.dll
del %1\libfreespace.dll
