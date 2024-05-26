@echo off

setlocal enableextensions
set TOOLCHAIN=%1
set CMAKE_GENERATOR=%2

set BAT_DIR=%~dp0
set BUILD_DIR=%BAT_DIR%\build
set BUILD_TOOLCHAIN_DIR=%BAT_DIR%\build\%TOOLCHAIN%

if not exist %BUILD_DIR% mkdir %BUILD_DIR% || exit /b 1
if not exist %BUILD_TOOLCHAIN_DIR% mkdir %BUILD_TOOLCHAIN_DIR% || exit /b 1

cd /d %BUILD_TOOLCHAIN_DIR% || exit /B 1

cmake.exe ..\..\. -G %CMAKE_GENERATOR% || exit /B 1