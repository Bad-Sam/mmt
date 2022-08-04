@echo off

:::::: Constants ::::::
setlocal EnableDelayedExpansion

:: Directories
set "BUILD_DIR=build\"
set "OBJ_DIR=%BUILD_DIR%obj\"
set "ASM_DIR=%BUILD_DIR%asm\"
set "SRC_DIR=src\"

:: Files
set "PROJECT_BINARY=%BUILD_DIR%mem.exe"
set "CONFIG_BINARY=%BUILD_DIR%gen_config.exe"
set "CONFIG_FILE=%BUILD_DIR%macro_config.txt"
set "PROJECT_SRC="
set "CONFIG_SRC=%BUILD_DIR%gen_config.c"
set "OBJ="

:: Compilation
set "COMP=clang"
set "INCLUDE_DIR=-I%SRC_DIR%"
:: MMT_MAX_REQUIRED_ALIGNMENT is expected to be in [8, 256]
set "MACRO=-DMMT_ENABLE_DEBUG_CHECKS=1 -DMMT_MAX_REQUIRED_ALIGNMENT=256"
set "WARNING=-W -Wall -Werror -Wextra -Wshadow"
set "OPTI=-O3"
set "ASM=-fverbose-asm -masm=intel"
set "LIBRARIES="

:::::: Rules ::::::
if "%1"=="" (
    cls
    if not exist %CONFIG_FILE% (
        call :gen_config
    )
    call :find_src
    call :config_macros
    call :compile
    exit /B 0
) else if "%1"=="config" (
    %COMP% %OPTI% %CONFIG_SRC% -o %CONFIG_BINARY%

    if exist %CONFIG_BINARY% (
        %CONFIG_BINARY% %CONFIG_FILE%
    )
    exit /B 0
) else if "%1"=="asm" (
    call :find_src
    call :config_macros
    call :asm
    exit /B 0
) else if "%1"=="run" (
    if exist %PROJECT_BINARY% (
        %PROJECT_BINARY%
    ) else (
        echo %PROJECT_BINARY% doesn't exist. Use "build"
    )
    exit /B 0
) else if "%1"=="clean" (
    rmdir /Q /S %ASM_DIR% %OBJ_DIR% 2> nul
    del /Q /F %PROJECT_BINARY%
    echo %ASM_DIR%, %OBJ_DIR% and %PROJECT_BINARY% removed
    exit /B 0
) else (
    echo Undefined rule
    exit /B 0
)


:::::: Functions ::::::
:: Compile the config generator and generate the config file, which consists of pre-computed macros
:gen_config
    if not exist %CONFIG_BINARY% (
        %COMP% %OPTI% %CONFIG_SRC% -o %CONFIG_BINARY%
    )
    
    if exist %CONFIG_BINARY% (
        %CONFIG_BINARY% %CONFIG_FILE%
    )
    exit /B 0


:: Fetch any *.c file in SRC_DIR
:find_src
    for /R %SRC_DIR% %%f in (*.c) do (
        set "file=%%f"
        set "PROJECT_SRC=!PROJECT_SRC!!file! "
    )
    exit /B 0


:: Fetch the pre-computed macros in CONFIG_FILE
:config_macros
    for /F "delims=" %%l in (!CONFIG_FILE!) do (
        set "MACRO=!MACRO! -D%%l"
    )
    exit /B 0

:: Compile to assembly
:asm
    setlocal EnableDelayedExpansion

    :: Create the build and objects directories if they do not exist
    mkdir %BUILD_DIR% 2> nul
    mkdir %ASM_DIR% 2> nul

    echo Compiling to asm...
    for %%i in (!PROJECT_SRC!) do (
        set "src_file=%%~nxi"
        set "asm_file=!src_file:.c=.asm!"
        set "asm_file=%~dp0%!ASM_DIR!!asm_file!"
        
        %COMP% %INCLUDE_DIR% %MACRO% %WARNING% %OPTI% %ASM% -S %%i -o !asm_file!
    )
    echo Done
    exit /B 0


:: Compile to binary
:compile
    setlocal EnableDelayedExpansion

    :: Create the build and objects directories if they do not exist
    mkdir %BUILD_DIR% 2> nul
    mkdir %OBJ_DIR% 2> nul

    echo Compiling to %PROJECT_BINARY%...
    echo(
    set "startTime=%time: =0%"

    ::Isolate the file and its extension from the absolute path to make its .o file path
    for %%i in (!PROJECT_SRC!) do (
        set "src_file=%%~nxi"
        set "obj_file=!src_file:.c=.o!"
        set "obj_file=%~dp0%!OBJ_DIR!!obj_file!"
        set "OBJ=!OBJ!!obj_file! "
        
        %COMP% %INCLUDE_DIR% %MACRO% %WARNING% %OPTI% -c %%i -o !obj_file!
    )

    %COMP% %LIBRARIES% !OBJ! -o %PROJECT_BINARY%

    :: https://stackoverflow.com/a/9935540
    set "endTime=%time: =0%"
    set "end=!endTime:%time:~8,1%=%%100)*100+1!" & set "start=!startTime:%time:~8,1%=%%100)*100+1!"
    set /A "elap=((((10!end:%time:~2,1%=%%100)*60+1!%%100)-((((10!start:%time:~2,1%=%%100)*60+1!%%100), elap-=(elap>>31)*24*60*60*100"
    set /A "cc=elap%%100+100,elap/=100,ss=elap%%60+100,elap/=60,mm=elap%%60+100,hh=elap/60+100"

    echo Done
    echo Compilation time: %hh:~1%h%time:~2,1%%mm:~1%min%time:~2,1%%ss:~1%%time:~8,1%%cc:~1%s
    exit /B 0
