@echo off
setlocal enabledelayedexpansion

if "%~1"=="" (
    echo [!] ERROR: No target .3dsx file provided.
    echo Usage: build.bat ^<path_to_3dsx^>
    echo Example: build.bat game.3dsx
    pause
    exit /b 1
)

set INPUT_3DSX=%~1
set TOOLS_DIR=.\tools

if not exist "%INPUT_3DSX%" (
    echo [!] ERROR: Could not find target file: %INPUT_3DSX%
    pause
    exit /b 1
)

echo ===================================================
echo Building Among Us 3DS CIA Package
echo Target Executable: %INPUT_3DSX%
echo ===================================================

set TITLE=Among Us 3DS
set AUTHOR=OmgRod
set DESCRIPTION=An online and local party game of teamwork and betrayal for 4-15 players...in space!
set PRODUCT_CODE=CTR-P-A3DS
set TITLE_ID=000400000A3DS000

set ICON_PNG=amongus.png
set SMDH_OUT=icon.smdh
set BANNER_OUT=banner.bnr
set CXI_OUT=game.cxi
set CIA_OUT=AmongUs3DS.cia

echo.
echo [1/4] Generating SMDH Icon metadata...
"%TOOLS_DIR%\bannertool.exe" makesmdh ^
    -s "%TITLE%" ^
    -l "%DESCRIPTION%" ^
    -a "%AUTHOR%" ^
    -i "%ICON_PNG%" ^
    -o "%SMDH_OUT%"

if %ERRORLEVEL% NEQ 0 (
    echo Error generating SMDH icon!
    goto :FAIL
)

echo.
echo [2/4] Generating 3DS Banner...
"%TOOLS_DIR%\bannertool.exe" makebanner ^
    -i "%ICON_PNG%" ^
    -o "%BANNER_OUT%"

if %ERRORLEVEL% NEQ 0 (
    echo Error generating banner!
    goto :FAIL
)

echo.
echo [3/4] Converting .3dsx to .cxi executable container...
"%TOOLS_DIR%\cxitool.exe" ^
    -n "%TITLE%" ^
    -c %PRODUCT_CODE% ^
    -t %TITLE_ID% ^
    -b "%BANNER_OUT%" ^
    "%INPUT_3DSX%" "%CXI_OUT%"

if %ERRORLEVEL% NEQ 0 (
    echo Error executing cxitool!
    goto :FAIL
)

echo.
echo [4/4] Packaging .cxi into final .cia installer...
"%TOOLS_DIR%\makerom.exe" ^
    -f cia ^
    -o "%CIA_OUT%" ^
    -target t ^
    -i "%CXI_OUT%":0:0 ^
    -icon "%SMDH_OUT%" ^
    -ignoresign

if %ERRORLEVEL% NEQ 0 (
    echo Error executing makerom!
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
echo [!] BUILD FAILED. Ensure tools exist in .\tools\ directory.
pause

:END
endlocal