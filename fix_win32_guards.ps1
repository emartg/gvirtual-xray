<#
  Fixes the "#if WIN32" / "#if !WIN32" preprocessor bug in gVirtualXRay.

  Converts:
    #if WIN32    ->  #ifdef WIN32
    #if !WIN32   ->  #ifndef WIN32

  These are existence checks, which work correctly whether WIN32 is
  defined with a value (e.g. -DWIN32=1) or defined blank (-DWIN32),
  unlike the original "#if WIN32" which breaks under g++/MinGW when
  WIN32 expands to nothing.

  Any #if/#elif line that mixes WIN32 into a larger expression
  (e.g. "#if WIN32 && FOO") is left UNTOUCHED and printed in a
  review list at the end -- those need "defined(WIN32)" substituted
  in by hand, not a blind #ifdef swap, since #ifdef can't carry the
  rest of the expression.

  BACK UP FIRST (or make sure this is a clean git checkout so you can
  "git diff" / "git checkout -- ." to inspect or undo). This edits
  files in place.

  Usage:
    .\fix_win32_guards.ps1
    .\fix_win32_guards.ps1 -Root "C:\Dev\gVirtualXray"
#>

param(
    [string]$Root = "C:\Dev\gVirtualXray"
)

if (-not (Test-Path $Root)) {
    Write-Host "Path not found: $Root" -ForegroundColor Red
    exit 1
}

$files = Get-ChildItem -Path $Root -Recurse -Include *.h,*.hpp,*.inl,*.cxx,*.cpp,*.c

$patchedCount = 0
$reviewList = @()

foreach ($file in $files) {
    $lines = Get-Content -Path $file.FullName
    $changed = $false

    for ($i = 0; $i -lt $lines.Count; $i++) {
        $line = $lines[$i]

        if ($line -match '^(?<indent>\s*#\s*)if\s+!\s*WIN32\s*(//.*)?$') {
            $lines[$i] = $line -replace '^(?<indent>\s*#\s*)if\s+!\s*WIN32', '${indent}ifndef WIN32'
            $changed = $true
        }
        elseif ($line -match '^(?<indent>\s*#\s*)if\s+WIN32\s*(//.*)?$') {
            $lines[$i] = $line -replace '^(?<indent>\s*#\s*)if\s+WIN32', '${indent}ifdef WIN32'
            $changed = $true
        }
        elseif ($line -match '#\s*(if|elif)\b.*\bWIN32\b' -and $line -notmatch 'defined\s*\(?\s*WIN32') {
            $reviewList += "$($file.FullName):$($i+1): $line"
        }
    }

    if ($changed) {
        Set-Content -Path $file.FullName -Value $lines
        $patchedCount++
        Write-Host "Patched: $($file.FullName)"
    }
}

Write-Host ""
Write-Host "Done. Patched $patchedCount file(s)." -ForegroundColor Green

if ($reviewList.Count -gt 0) {
    Write-Host ""
    Write-Host "These lines mix WIN32 into a larger expression and were left untouched -- fix by hand (wrap WIN32 in defined(...)):" -ForegroundColor Yellow
    $reviewList | ForEach-Object { Write-Host "  $_" -ForegroundColor Yellow }
}
