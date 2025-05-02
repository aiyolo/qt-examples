@echo off
set __VSCMD_ARG_NO_LOGO=1
set VSCMD_SKIP_SENDTELEMETRY=1
set "VSCMD_START_DIR=%CD%"

set "VS_VERSION=%1"
if /I "%VS_VERSION%"=="2019" (
    echo vcvars.bat: Activating environment Visual Studio 16 - amd64 - winsdk_version=None - vcvars_ver=14.2
    call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary/Build/vcvarsall.bat" amd64 -vcvars_ver=14.2
    if errorlevel 1 (
        echo Error: Failed to activate Visual Studio 2019 environment.
        exit /b 1
    )
) else (
    echo vcvars.bat: Activating environment Visual Studio 17 - amd64 - winsdk_version=None - vcvars_ver=14.2
    call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC/Auxiliary/Build/vcvarsall.bat" amd64 -vcvars_ver=14.2
    if errorlevel 1 (
        echo Error: Failed to activate Visual Studio 2022 environment.
        exit /b 1
    )
)