# Fix IconsForkAwesome → IconsFontAwesome6 references
# Run this script from EnJinn/EnJinn directory

$files = Get-ChildItem -Path ".\core" -Recurse -Include *.h,*.cpp

foreach ($file in $files) {
    $content = Get-Content $file.FullName -Raw
    
    if ($content -match 'Icons Fork Awesome') {
        Write-Host "Updating: $($file.FullName)"
        
        # Replace all variations
        $content = $content -replace 'IconsForkAwesome\.h', 'IconsFontAwesome6.h'
        $content = $content -replace '#include "IconsForkAwesome\.h"', '#include "IconsFontAwesome6.h"'
        $content = $content -replace '#include <IconsForkAwesome\.h>', '#include <IconsFontAwesome6.h>'
        
        # Save
        Set-Content -Path $file.FullName -Value $content -NoNewline
    }
}

Write-Host "Done! All files updated."
