@echo off
echo Starting Database Admin Application...
echo.

cd /d "%~dp0\..\..\build\apps\databaseadmin\Debug"

if not exist "databaseadmin.exe" (
    echo Error: databaseadmin.exe not found!
    echo Please build the application first using build.bat
    pause
    exit /b 1
)

echo Launching Database Admin...
echo.
databaseadmin.exe

if %errorlevel% neq 0 (
    echo Application exited with error code: %errorlevel%
)

pause
