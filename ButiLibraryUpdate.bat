mkdir output
mkdir output\Debug
mkdir output\Release
mkdir output\Debug_imguiEditor
mkdir output\Release_imguiEditor
call .\ButiGitUpdate.bat
set FILE_ID=1vkgzAuLeAu0quQkGypV-2r81FPLQkriL
curl -L "https://drive.google.com/uc?export=download&id=%FILE_ID%" -o dll.zip
set FILE_ID=1LObD7mhQ03WHranGUIsoeaCsCZL2cqzr
curl -c /tmp/cookie "https://drive.google.com/uc?export=download&id=%FILE_ID%" /dev/null
set CODE="%(awk '/_warning_/ {print %NF%}' /tmp/cookie)%"
curl -Lb /tmp/cookie "https://drive.google.com/uc?export=download&confirm=%CODE%&id=%FILE_ID%" -o lib.zip
call powershell -command "Expand-Archive -Force ./dll.zip"
call powershell -command "Expand-Archive -Force ./lib.zip"
cd ..\
xcopy /y /d /e Libs\dll\dll\ Libs\dll
xcopy /y /d /e Libs\lib\lib\ Libs\lib
rmdir /s /q Libs\dll\dll
rmdir /s /q Libs\lib\lib
del Libs\dll.zip
del Libs\lib.zip
xcopy /y /d Libs\dll\Debug\ output\Debug\
xcopy /y /d Libs\dll\Debug_imguiEditor\ output\Debug_imguiEditor\
xcopy /y /d Libs\dll\Release\ output\Release\
xcopy /y /d Libs\dll\Release_imguiEditor\ output\Release_imguiEditor\

pause