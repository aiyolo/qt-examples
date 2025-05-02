# param(
#     [ValidateSet("Debug", "Release")]
#     [string]$buildType = "Debug"
# )
# conan install . -s build_type=Release --build=missing -c tools.env.virtualenv:powershell=powershell
conan install . -s build_type=Debug `
    --build=missing `
    -pr conan/profile/ninja-win64 `
    -d conan/deployers/custom_deploy `
    --deployer-folder third_party
