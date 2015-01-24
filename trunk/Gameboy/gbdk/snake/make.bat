@ECHO OFF

if NOT defined GBDK_LCC_PATH (
	SET "GBDK_LCC_PATH=D:\Gameboy.dev\gbdk\bin\lcc.exe"
	@echo INFO: GBDK_LCC_PATH undefined, use value of the current bat file "%GBDK_LCC_PATH%"
)

SET "BUILD_OPTIONS=-Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -c"
SET "LINK_OPTIONS=-Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG"

REM you want to change this to the filename to compile XXXX.c (without extension)
SET "CFILE=snake"
REM in case you need to link other files, list it here
SET "OFILES="

@echo Building ...
%GBDK_LCC_PATH% %BUILD_OPTIONS% -o %CFILE%.o %CFILE%.c
if NOT errorlevel 0 (
   echo Failure Reason Given is %errorlevel%
   pause
   exit /b %errorlevel%
)

@echo Linking ...
%GBDK_LCC_PATH% %LINK_OPTIONS% -o %CFILE%.gb %CFILE%.o %OFILES%
if NOT errorlevel 0 (
   echo Failure Reason Given is %errorlevel%
   pause
   exit /b %errorlevel%
)


@echo Run !
start %CFILE%.gb
