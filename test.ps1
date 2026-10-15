$projectRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
$srcDir = Join-Path $projectRoot "src"
$testsDir = Join-Path $projectRoot "tests"
$outputPath = Join-Path $testsDir "test_allocator.exe"

& gcc -std=c11 -Wall -Wextra -Wpedantic -O2 `
    -I$srcDir `
    (Join-Path $testsDir "test_allocator.c") `
    (Join-Path $srcDir "allocator.c") `
    -o $outputPath

if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE
}

& $outputPath
