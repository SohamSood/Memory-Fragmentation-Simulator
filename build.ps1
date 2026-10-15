$projectRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
$srcDir = Join-Path $projectRoot "src"
$outputPath = Join-Path $projectRoot "memory_simulator.exe"

& gcc -std=c11 -Wall -Wextra -Wpedantic -O2 `
    -I$srcDir `
    (Join-Path $srcDir "main.c") `
    (Join-Path $srcDir "allocator.c") `
    -o $outputPath

if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE
}

Write-Host "Build complete: $outputPath"
