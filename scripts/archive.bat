@echo off

:version
set /P VERSION=Version : 
if "%VERSION%" == "" exit

set NAMEDDATE=%DATE:~6,4%%DATE:~3,2%%DATE:~0,2%
set OUTPUT=tbeditor-%VERSION%-%NAMEDDATE%.7z

title Making %OUTPUT%

echo.
echo Making %OUTPUT%
echo.

7z a "%OUTPUT%" ../release/* -mx=9 -xr!*.lnk -xr!*.log

:END
pause > nul
