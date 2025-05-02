# vcvars_selector.ps1: PowerShell script to call vcvars.bat for Visual Studio 2019 or 2022 environment
# Usage: .\vcvars_selector.ps1 [2019|2022]
# Default: Activates Visual Studio 2022 if no parameter is provided

param (
  [string]$vsVersion = "2022" 
)

$env:__VSCMD_ARG_NO_LOGO = "1"
$env:VSCMD_SKIP_SENDTELEMETRY = "1"
$env:VSCMD_START_DIR = Get-Location

$displayVersion = $null
switch ($vsVersion.ToLower()) {
  "2019" { $displayVersion = "2019 (version 16)" }
  "2022" { $displayVersion = "2022 (version 17)" }
  default {
    Write-Host "Error: Invalid version '$vsVersion'. Use '2019' or '2022'."
    exit 1
  }
}

$batchScript = Join-Path -Path $PSScriptRoot -ChildPath "vcvars.bat"

if (-not (Test-Path $batchScript)) {
  Write-Host "Error: Batch script 'vcvars.bat' not found in the same directory ($PSScriptRoot)."
  exit 1
}

Write-Host "vcvars_selector.ps1: Activating environment Visual Studio $displayVersion"
Push-Location "$PSScriptRoot"
try {
  $envVars = cmd /c "`"$batchScript`" $vsVersion & set"
  if ($LASTEXITCODE -ne 0) {
    Write-Host "Error: Failed to activate Visual Studio $displayVersion environment."
    exit 1
  }
  foreach ($line in $envVars) {
    if ($line -match "=") {
      $v = $line -split "=", 2
      Set-Item -Force -Path "ENV:\$($v[0])" -Value "$($v[1])"
    }
  }
}
finally {
  Pop-Location
}