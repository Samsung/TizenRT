# Fix multiple definition linker errors by wrapping file-scope code in anonymous namespaces
# Usage: powershell -ExecutionPolicy Bypass -File fix_multiple_defs.ps1

$baseDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$algoDir = Join-Path $baseDir "std\algorithms"
$fixedCount = 0

function Fix-PassCppFile {
    param([string]$filepath)
    
    $content = Get-Content $filepath -Raw -Encoding UTF8
    $lines = $content -split "`n"
    
    # Skip if already has anonymous namespace
    foreach ($line in $lines) {
        $stripped = $line.Trim()
        if ($stripped -eq 'namespace {' -or $stripped.StartsWith('namespace { //')) {
            Write-Host "  SKIP (already has anonymous namespace): $filepath"
            return $false
        }
    }
    
    # Find entry point function
    $entryStart = -1
    for ($i = 0; $i -lt $lines.Count; $i++) {
        if ($lines[$i].Trim() -match '^int\s+tc_libcxx_\w+\s*\(') {
            $entryStart = $i
            break
        }
    }
    
    if ($entryStart -eq -1) {
        Write-Host "  SKIP (no entry point): $filepath"
        return $false
    }
    
    # Find last #include
    $lastInclude = -1
    for ($i = 0; $i -lt $lines.Count; $i++) {
        if ($lines[$i].Trim().StartsWith('#include')) {
            $lastInclude = $i
        }
    }
    
    if ($lastInclude -eq -1) {
        Write-Host "  SKIP (no includes): $filepath"
        return $false
    }
    
    # Find end of entry point function (matching braces)
    $entryEnd = -1
    $braceCount = 0
    $started = $false
    for ($i = $entryStart; $i -lt $lines.Count; $i++) {
        foreach ($ch in $lines[$i].ToCharArray()) {
            if ($ch -eq '{') { $braceCount++; $started = $true }
            elseif ($ch -eq '}') { $braceCount-- }
            if ($started -and $braceCount -eq 0) {
                $entryEnd = $i
                break
            }
        }
        if ($entryEnd -ne -1) { break }
    }
    
    if ($entryEnd -eq -1) { $entryEnd = $lines.Count - 1 }
    
    # Find first non-blank line after last include
    $codeStart = $lastInclude + 1
    while ($codeStart -lt $lines.Count -and $lines[$codeStart].Trim() -eq '') {
        $codeStart++
    }
    
    $hasCodeBefore = $codeStart -lt $entryStart
    
    # Check for code after entry point
    $hasCodeAfter = $false
    for ($i = $entryEnd + 1; $i -lt $lines.Count; $i++) {
        $stripped = $lines[$i].Trim()
        if ($stripped -ne '' -and -not $stripped.StartsWith('//')) {
            $hasCodeAfter = $true
            break
        }
    }
    
    if (-not $hasCodeBefore -and -not $hasCodeAfter) {
        Write-Host "  SKIP (no code to wrap): $filepath"
        return $false
    }
    
    $result = @()
    
    if ($hasCodeBefore) {
        $result += $lines[0..($codeStart - 1)]
        $result += 'namespace {'
        $result += $lines[$codeStart..($entryStart - 1)]
        $result += '} // namespace'
        $result += ''
    } else {
        $result += $lines[0..($entryStart - 1)]
    }
    
    $result += $lines[$entryStart..$entryEnd]
    
    if ($hasCodeAfter) {
        $result += ''
        $result += 'namespace {'
        $result += $lines[($entryEnd + 1)..($lines.Count - 1)]
        $result += '} // namespace'
    } else {
        $result += $lines[($entryEnd + 1)..($lines.Count - 1)]
    }
    
    $newContent = $result -join "`n"
    if ($newContent -eq $content) {
        Write-Host "  SKIP (no change): $filepath"
        return $false
    }
    
    [System.IO.File]::WriteAllText($filepath, $newContent, [System.Text.Encoding]::UTF8)
    Write-Host "  FIXED: $filepath"
    return $true
}

function Fix-CountNewH {
    param([string]$filepath)
    
    $content = Get-Content $filepath -Raw -Encoding UTF8
    
    if ($content.Contains('namespace {')) {
        Write-Host "  SKIP (already has anonymous namespace): $filepath"
        return $false
    }
    
    # Find #ifdef DISABLE_NEW_COUNT
    $pattern1 = '(?m)^#ifdef DISABLE_NEW_COUNT\s*$'
    # Find #endif // DISABLE_NEW_COUNT
    $pattern2 = '(?m)^#endif // DISABLE_NEW_COUNT\s*$'
    
    if ($content -notmatch $pattern1 -or $content -notmatch $pattern2) {
        Write-Host "  SKIP (patterns not found): $filepath"
        return $false
    }
    
    # Insert namespace { before #ifdef DISABLE_NEW_COUNT
    # Insert } // namespace after #endif // DISABLE_NEW_COUNT
    $idx1 = [regex]::Match($content, $pattern1).Index
    $idx2 = [regex]::Match($content, $pattern2).Index + [regex]::Match($content, $pattern2).Length
    
    $newContent = $content.Substring(0, $idx1) + "namespace {`n" + $content.Substring($idx1, $idx2 - $idx1) + "`n} // namespace`n" + $content.Substring($idx2)
    
    [System.IO.File]::WriteAllText($filepath, $newContent, [System.Text.Encoding]::UTF8)
    Write-Host "  FIXED: $filepath"
    return $true
}

# Fix all .pass.cpp files
Write-Host "Fixing algorithm test files..."
Get-ChildItem -Path $algoDir -Filter "*.pass.cpp" -Recurse | Sort-Object FullName | ForEach-Object {
    if (Fix-PassCppFile $_.FullName) {
        $script:fixedCount++
    }
}

# Fix count_new.h
Write-Host "`nFixing support/count_new.h..."
$countNewPath = Join-Path $baseDir "support\count_new.h"
if (Test-Path $countNewPath) {
    if (Fix-CountNewH $countNewPath) {
        $script:fixedCount++
    }
}

Write-Host "`nTotal files fixed: $fixedCount"