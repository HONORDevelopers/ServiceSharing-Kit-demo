cd /d "%~dp0"
set currentDir=%~dp0
set currentDir=%currentDir:\=\\%
set outputString="InstallLocation"="%currentDir%"
echo %outputString% >> demo_reg_Distributed.reg

regedit /s demo_reg_Distributed.reg

pause
