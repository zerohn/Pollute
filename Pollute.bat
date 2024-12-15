@echo off
set ENGINE_PATH="D:\1_Operation\1_EpicGames\UE_5.4\Engine\Binaries\Win64\UnrealEditor.exe"
set PROJECT_PATH="D:\1_Operation\Pollute\Pollute.uproject"

%ENGINE_PATH% %PROJECT_PATH% -windowed -game -log -resx=1280 -resy=720
pause
