@echo off
setlocal enabledelayedexpansion

REM Define the name of the executable
set "EXECUTABLE=dev-tictactoe.exe"

REM Terminate the application if it's already running
taskkill /IM %EXECUTABLE% /F >nul 2>&1

REM Initialize variables
set "SOURCE_FILES="
set "TARGET=dev-tictactoe"

REM Set the pattern for files to include
set "EXT=m.cpp"

REM Find all files with the specified pattern recursively and add them to the SOURCE_FILES variable
for /r %%i in (*.%EXT%) do (
    set "SOURCE_FILES=!SOURCE_FILES! %%i"
)

REM Compile the project
g++ -o %TARGET% main.cpp %SOURCE_FILES% -lgdi32

REM Check for compilation errors
if %ERRORLEVEL% NEQ 0 exit /b %ERRORLEVEL%

REM Run the executable
%TARGET%.exe
