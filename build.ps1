Param(
    [Parameter(Mandatory=$false)]
    [Switch]$clean,
    [Parameter(Mandatory=$false)]
    [Switch]$release
)

# if user specified clean, remove all build files
if ($clean.IsPresent)
{
    if (Test-Path -Path "build")
    {
        remove-item build -R
    }
}

if (($clean.IsPresent) -or (-not (Test-Path -Path "build")))
{
    $out = new-item -Path build -ItemType Directory
}

$buildType = "Debug"
if ($release.IsPresent) {
    $buildType = "RelWithDebInfo"
}

& cmake -G "Ninja" -DCMAKE_BUILD_TYPE="$buildType" -B build/
& cmake --build ./build
exit $LastExitCode