@echo off
setlocal enabledelayedexpansion

set INPUT_3DSX=AmongUs3DS.3dsx
if not "%~1"=="" set INPUT_3DSX=%~1

set TOOLS_DIR=.\tools

if not exist "%INPUT_3DSX%" (
    echo [!] ERROR: Could not find target file: %INPUT_3DSX%
    echo Please run 'make' in your devkitPro environment to compile AmongUs3DS.3dsx first.
    pause
    exit /b 1
)

echo ===================================================
echo Building Among Us 3DS CIA Package
echo Target Executable: %INPUT_3DSX%
echo ===================================================

set TITLE=AmongUs
set AUTHOR=OmgRod
set DESCRIPTION=An online and local party game of teamwork and betrayal for 4-15 players...in space!
set PRODUCT_CODE=CTR-P-A3DS
set TITLE_ID=000400000A3D5000

set ICON_PNG=amongus.png
set BANNER_PNG=banner.png
set AUDIO_WAV=audio.wav
set SMDH_OUT=icon.smdh
set BANNER_OUT=banner.bnr
set CXI_OUT=game.cxi
set CIA_OUT=AmongUs3DS.cia

:: Step 0: Generate minimal silent WAV if no audio track exists
if not exist "%AUDIO_WAV%" (
    <nul set /p="RIFF$   WAVEfmt     ^    DATA    " > "%AUDIO_WAV%"
)

echo.
echo [1/4] Generating custom SMDH Icon metadata...
"%TOOLS_DIR%\bannertool.exe" makesmdh ^
    -s "Among Us" ^
    -l "%DESCRIPTION%" ^
    -p "%AUTHOR%" ^
    -i "%ICON_PNG%" ^
    -o "%SMDH_OUT%"

if %ERRORLEVEL% NEQ 0 (
    echo Error generating SMDH icon!
    goto :FAIL
)

echo.
echo [2/4] Generating custom 3DS Banner ^& Audio...
"%TOOLS_DIR%\bannertool.exe" makebanner ^
    -i "%BANNER_PNG%" ^
    -a "%AUDIO_WAV%" ^
    -o "%BANNER_OUT%"

if %ERRORLEVEL% NEQ 0 (
    echo Error generating banner!
    goto :FAIL
)

echo.
echo [3/4] Converting executable into NCCH container (.cxi)...
"%TOOLS_DIR%\cxitool.exe" ^
    -n %TITLE% ^
    -c %PRODUCT_CODE% ^
    -t %TITLE_ID% ^
    -b "%BANNER_OUT%" ^
    "%INPUT_3DSX%" "%CXI_OUT%"

if %ERRORLEVEL% NEQ 0 (
    echo Error executing cxitool!
    goto :FAIL
)

echo.
echo [4/4] Packaging .cxi, RSF permissions, and custom SMDH into CIA...
"%TOOLS_DIR%\makerom.exe" ^
    -f cia ^
    -o "%CIA_OUT%" ^
    -rsf app.rsf ^
    -target t ^
    -content "%CXI_OUT%":0:0 ^
    -icon "%SMDH_OUT%" ^
    -banner "%BANNER_OUT%" ^
    -ignoresign

if %ERRORLEVEL% NEQ 0 (
    echo Error building CIA!
    goto :FAIL
)

echo.
echo ===================================================
echo SUCCESS! Output created: %CIA_OUT%
echo Install this file on your 3DS using FBI.
echo ===================================================
goto :END

:FAIL
echo.
echo [!] BUILD FAILED. Check tools and files.
pause

:END
endlocal
