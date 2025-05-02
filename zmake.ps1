param (
    [Parameter(Position = 0, Mandatory = $false)]
    [ValidateSet("all", "deps", "activate", "build", "clean", "config", "deactivate")]
    [string]$action,

    [ValidateSet("Debug", "Release")]
    [string]$buildType = "Debug"
)

$cwd = Get-Location
$scriptDir = "$cwd\conan\scripts"
$scriptsDir = $scriptDir

$scriptMap = @{
    "all"        = @{ Path = "$scriptsDir\all.ps1"; Description = "Running all scripts" }
    "deps"       = @{ Path = "$scriptsDir\install_deps.ps1"; Description = "Installing dependencies" }
    "activate"   = @{ Path = "$scriptsDir\activate_env.ps1"; Description = "Activating environment" }
    "build"      = @{ Path = "$scriptsDir\build.ps1"; Description = "Building with buildType=$buildType" }
    "run"        = @{ Path = "$scriptsDir\run.ps1"; Description = "Running" }
    "clean"      = @{ Path = "$scriptsDir\clean_build.ps1"; Description = "Cleaning build" }
    "config"     = @{ Path = "$scriptsDir\configure.ps1"; Description = "Configuring" }
    "deactivate" = @{ Path = "$scriptsDir\deactivate_env.ps1"; Description = "Deactivating environment" }
}

# Set default action to "all" if no action is specified
$selectedAction = if ([string]::IsNullOrEmpty($action)) { "all" } else { $action }

if (-not $scriptMap.ContainsKey($selectedAction)) {
    Write-Error "Invalid action: $selectedAction"
    exit 1
}

$scriptInfo = $scriptMap[$selectedAction]

Write-Host "--$($scriptInfo.Description)..." -ForegroundColor Green
Write-Host "--Script: $($scriptInfo.Path)..." -ForegroundColor Green

if ($selectedAction -eq "build" -or $selectedAction -eq "install") {
    & $scriptInfo.Path -buildType $buildType
}
else {
    & $scriptInfo.Path
}
