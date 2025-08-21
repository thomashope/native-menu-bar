@echo off
setlocal enabledelayedexpansion

rem Compiler flags
set CFLAGS=/nologo /W4 /WX
set LFLAGS=/link user32.lib
set FILES=test.c ..\native_menu_bar.c

rem Ensure environment is set up for MSVC (this assumes you're in a Developer Command Prompt)
rem If not, call "vcvarsall.bat" first

call :run cl %CFLAGS% /std:c99 %FILES% /Fe:test_c99.exe %LFLAGS%
call :run cl %CFLAGS% /std:c17 %FILES% /Fe:test_c23.exe %LFLAGS%
call :run cl %CFLAGS% /std:c++14 %FILES% /TP /Fe:test_cpp98.exe %LFLAGS%
call :run cl %CFLAGS% /std:c++20 %FILES% /TP /Fe:test_cpp23.exe %LFLAGS%

goto :eof

:run
    echo Running: %*
    %*
    if errorlevel 1 (
        echo Command failed: %*
        exit /b 1
    )
    goto :eof
