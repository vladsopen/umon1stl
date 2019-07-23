@echo off
c:
cd c:\xv\ccide\em\emsdk
:call emsdk update
:call emsdk install latest
:call emsdk activate latest
call emsdk_env.bat

set EMSDK=C:/xv/ccide/em/emsdk
set EMSDK_MASTER=%EMSDK%/emscripten\1.38.15
if not exist %EMSDK_MASTER% goto badver

set deploy=C:\xa\xampp\htdocs\telex\umon

SET PATH=%PATH%;%EMSDK_MASTER%

SET PATH=%PATH%;C:\xv\ccide\em\emsdk
SET PATH=%PATH%;C:\xv\ccide\em\emsdk\clang\e1.38.15_64bit
SET PATH=%PATH%;C:\xv\ccide\em\emsdk\node\8.9.1_64bit\bin
SET PATH=%PATH%;C:\xv\ccide\em\emsdk\python\2.7.13.1_64bit\python-2.7.13.amd64
SET PATH=%PATH%;C:\xv\ccide\em\emsdk\emscripten\1.38.15

SET PATH=%PATH%;C:\xv\ccide\em\cmake\bin

call emcc -v

:echo on
cd %~dp0%goup%
set goup=..\..\
set buildresult=build-emscripten
mkdir %goup%%buildresult%
copy Umon-template.html %goup%%buildresult%\Umon.html
cd %goup%%buildresult%

set CXXFLAGS=
set CXXFLAGS=%CXXFLAGS% -Os
set CXXFLAGS=%CXXFLAGS% -s ASSERTIONS=2
set CXXFLAGS=%CXXFLAGS% -s DEMANGLE_SUPPORT=1
set CXXFLAGS=%CXXFLAGS% -Werror
set CXXFLAGS=%CXXFLAGS% -Wall
set CXXFLAGS=%CXXFLAGS% -Wextra
set CXXFLAGS=%CXXFLAGS% -Wpedantic
set CXXFLAGS=%CXXFLAGS% -Wnon-virtual-dtor
set CXXFLAGS=%CXXFLAGS% -Wuninitialized
set CXXFLAGS=%CXXFLAGS% -Wcast-align
:set CXXFLAGS=%CXXFLAGS% -Wunused
set CXXFLAGS=%CXXFLAGS% -Wno-unused-lambda-capture
set CXXFLAGS=%CXXFLAGS% -Wno-sign-conversion
set CXXFLAGS=%CXXFLAGS% -Winit-self
set CXXFLAGS=%CXXFLAGS% -Wformat=2
:set CXXFLAGS=%CXXFLAGS% -Wlogical-op
:set CXXFLAGS=%CXXFLAGS% -Wuseless-cast
set CXXFLAGS=%CXXFLAGS% -Wshadow
set CXXFLAGS=%CXXFLAGS% -Wnull-dereference
:set CXXFLAGS=%CXXFLAGS% -Wduplicated-cond
:set CXXFLAGS=%CXXFLAGS% -Wduplicated-branches
:set CXXFLAGS=%CXXFLAGS% -Wlifetime
set CXXFLAGS=%CXXFLAGS% -Wbool-conversions

echo.
echo C++ flags:
echo %CXXFLAGS%
echo.

:dir ..\public\with_emscripten

cmake ^
    -DCMAKE_TOOLCHAIN_FILE=%EMSDK_MASTER%/cmake/Modules/Platform/Emscripten.cmake ^
    -DCMAKE_BUILD_TYPE=RelWithDebInfo ^
    -G "MinGW Makefiles" ^
    -B build-em ^
    ../public/with_emscripten

:call emcmake

C:\Qt\Qt5.5.1\Tools\mingw492_32\bin\mingw32-make.exe

if %errorlevel%==0 (
    if exist %deploy% (
        echo Deploying...
        copy Umon.js %deploy%
        copy Umon.wasm %deploy%
        copy Umon.wast %deploy%
        copy Umon.html %deploy%
    )
)

echo errorlevel=%errorlevel%

pause

goto end

:badver
echo ****** VL: Check if you need to update emscripten version in the .bat file!
pause

:end
