param(
    [string]$buildType = "Debug"
)
$cwd = Get-Location
& "$cwd\conan\generators\conanbuild.ps1"
& "$cwd\conan\generators\conanrun.ps1"
