param(
    [string]$buildType = "Debug"
)
$cwd = Get-Location
& "$cwd\build\generators\deactivate_conanbuild.ps1"
& "$cwd\build\generators\deactivate_conanrun.ps1"