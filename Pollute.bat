@echo off
set ENGINE_PATH="E:\UE_5.4\Engine\Binaries\Win64\UnrealEditor.exe"
set PROJECT_PATH="E:\Project\Network\Pollute\Pollute.uproject"

%ENGINE_PATH% %PROJECT_PATH% -windowed -game -log -resx=1280 -resy=720
pause
