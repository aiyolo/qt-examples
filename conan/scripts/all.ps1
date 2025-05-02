$cwd = Get-Location
Write-Host "--Installing dependencies..." -ForegroundColor Green
& "$cwd\conan\scripts\install_deps.ps1"
Write-Host "--Activating environment..." -ForegroundColor Green
& "$cwd\conan\scripts\activate_env.ps1"
Write-Host "--Configuring..." -ForegroundColor Green
& "$cwd\conan\scripts\configure.ps1"
Write-Host "--Building..." -ForegroundColor Green
& "$cwd\conan\scripts\build.ps1"
