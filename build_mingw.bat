@echo off
setlocal enableextensions

set CMAKE_PREFIX_PATH=D:\Qt\6.7.1\mingw_64

call init_project.bat mingw "MinGW Makefiles" || exit /B 1
cd build/mingw || exit /B 1
mingw32-make TicTacToe -j5 || exit /B 1
mingw32-make GameLib -j5 || exit /B 1
