@echo off
echo ========================================
echo   EnJinn Build Script
echo ========================================
echo.

REM Check if vcpkg is installed
if not defined VCPKG_ROOT (
    echo [ERROR] VCPKG_ROOT environment variable not set
    echo.
    echo Please install vcpkg and set VCPKG_ROOT:
    echo   1. git clone https://github.com/microsoft/vcpkg.git C:\vcpkg
    echo   2. cd C:\vcpkg
    echo   3. .\bootstrap-vcpkg.bat
    echo   4. setx VCPKG_ROOT "C:\vcpkg"
    echo.
    pause
    exit /b 1
)

echo [INFO] vcpkg found at: %VCPKG_ROOT%
echo.

REM Navigate to EnJinn directory
cd /d "%~dp0\EnJinn"

REM Create build directory
if not exist build (
    echo [INFO] Creating build directory...
    mkdir build
)

cd build

REM Configure with CMake
echo [INFO] Configuring project with CMake...
echo [INFO] Dependencies will be automatically installed from vcpkg.json
echo.

cmake .. -DCMAKE_TOOLCHAIN_FILE=%VCPKG_ROOT%/scripts/buildsystems/vcpkg.cmake

if errorlevel 1 (
    echo.
    echo [ERROR] CMake configuration failed!
    echo Please check the error messages above.
    pause
    exit /b 1
)

echo.
echo [INFO] Configuration successful!
echo.

REM Build the project
echo [INFO] Building project (Debug configuration)...
echo.

cmake --build . --config Debug

if errorlevel 1 (
    echo.
    echo [ERROR] Build failed!
    echo Please check the error messages above.
    pause
    exit /b 1
)

echo.
echo ========================================
echo   Build Complete!
echo ========================================
echo.
echo Executable location:
echo   %CD%\Debug\EnJinnCore.exe
echo.
echo To run the engine:
echo   cd build\Debug
echo   .\EnJinnCore.exe
echo.
pause
