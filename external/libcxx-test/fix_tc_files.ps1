# Fix tc_*.cpp/hpp files to only reference functions from .pass.cpp files that exist on disk
# The .pass.cpp files define functions like tc_libcxx_utilities_pair_astuple_get_const
# The tc files must call these exact function names

$baseDir = Split-Path -Parent $MyInvocation.MyCommand.Path

function Get-TestFunctions($category) {
    $searchPath = Join-Path $baseDir "std\$category"
    if (-not (Test-Path $searchPath)) { return @() }
    
    $funcs = @()
    Get-ChildItem -Path $searchPath -Recurse -Filter '*.pass.cpp' | ForEach-Object {
        $m = Select-String -Path $_.FullName -Pattern 'int (tc_libcxx_\w+)\(void\)' | Select-Object -First 1
        if ($m) {
            $funcs += $m.Matches[0].Groups[1].Value
        }
    }
    return $funcs | Sort-Object -Unique
}

function Generate-TcFiles($category, $funcs) {
    $catDir = Join-Path $baseDir "std\$category"
    if (-not (Test-Path $catDir)) { 
        New-Item -ItemType Directory -Path $catDir -Force | Out-Null
    }
    
    $guardName = "TC_$($category.ToUpper())_HPP"
    
    # Generate .hpp
    $hpp = "#ifndef $guardName`n"
    $hpp += "#define $guardName`n`n"
    $hpp += "#include <tinyara/config.h>`n`n"
    foreach ($f in $funcs) {
        $hpp += "extern int $f(void);`n"
    }
    $hpp += "`n#endif /* $guardName */`n"
    
    # Generate .cpp
    $cpp = "#include ""tc_$category.hpp""`n`n"
    $cpp += "extern ""C"" {`n"
    $cpp += "#include <tc_common.h>`n"
    $cpp += "}`n`n"
    $cpp += "extern int tc_$category`_main(void)`n{`n"
    if ($funcs.Count -gt 0) {
        foreach ($f in $funcs) {
            $cpp += "	$f();`n"
        }
    } else {
        $cpp += "	/* No test files available for this category */`n"
    }
    $cpp += "}`n"
    
    $hppPath = Join-Path $catDir "tc_$category.hpp"
    $cppPath = Join-Path $catDir "tc_$category.cpp"
    
    Set-Content -Path $hppPath -Value $hpp -NoNewline
    Set-Content -Path $cppPath -Value $cpp -NoNewline
    
    Write-Host "Generated tc_$category.hpp/cpp with $($funcs.Count) test functions"
}

# Process each category
$categories = @('algorithms', 'utilities', 'containers', 'strings', 'iterators', 'diagnostics', 'numerics', 'thread')

foreach ($cat in $categories) {
    $funcs = Get-TestFunctions $cat
    Generate-TcFiles $cat $funcs
}

Write-Host "`nDone! tc files have been regenerated to match existing .pass.cpp files."