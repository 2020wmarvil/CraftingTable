@echo off
rem Runs a command inside an MSVC x64 developer environment.
rem Usage: scripts\msvc.cmd cmake --preset windows-msvc
setlocal
rem vswhere is called through PATH because "(x86)" breaks quoting inside for /f.
set "PATH=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer;%PATH%"
where vswhere.exe >nul 2>&1
if errorlevel 1 (
    echo error: vswhere.exe not found; install Visual Studio with the C++ workload 1>&2
    exit /b 1
)
set "VS_PATH="
for /f "usebackq delims=" %%i in (`vswhere.exe -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do set "VS_PATH=%%i"
if not defined VS_PATH (
    echo error: Visual Studio with C++ tools not found 1>&2
    exit /b 1
)
call "%VS_PATH%\VC\Auxiliary\Build\vcvars64.bat" >nul || exit /b 1
rem Expose the clang-format and clang-tidy that ship with Visual Studio.
set "PATH=%PATH%;%VS_PATH%\VC\Tools\Llvm\x64\bin"
%*
