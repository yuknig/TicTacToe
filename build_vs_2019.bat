@echo off
setlocal enableextensions

set CMAKE_PREFIX_PATH=D:\Qt\6.7.1\msvc2019_64

call init_project.bat msvc19 "Visual Studio 16" || exit /B 1

start build/msvc19/TicTacToe.sln