@echo off
echo Building Database Admin Application...
echo.

cd /d "%~dp0\..\.."

if not exist "build" (
    echo Creating build directory...
    mkdir build
)

cd build

echo Configuring with CMake...
cmake .. -G "Ninja"
if %errorlevel% neq 0 (
    echo CMake configuration failed!
    pause
    exit /b %errorlevel%
)

echo.
echo Building databaseadmin target...
cmake --build . --target databaseadmin
if %errorlevel% neq 0 (
    echo Build failed!
    pause
    exit /b %errorlevel%
)

echo.
echo Build completed successfully!
echo Executable location: build\apps\databaseadmin\Debug\databaseadmin.exe

echo.
echo To run the application:
echo cd build\apps\databaseadmin\Debug
echo databaseadmin.exe

pause
