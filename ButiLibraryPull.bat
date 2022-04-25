cd Libs\Library\ButiBulletWrap\
git pull
cd ..\ButiScript\
git pull
cd ..\ButiMemorySystem\
git pull
cd ..\ButiUtil\
git pull
cd ..\ButiMath\
git pull
cd ..\ButiEventSystem\
git pull
cd ..\ButiFont\
git pull
cd ..\ButiRendering_Dx12\
git pull
cd..\..\..\
xcopy /y /d "Libs\dll\Debug\" "output\UserSample\Debug\"
xcopy /y /d "Libs\dll\Debug_imguiEditor\" "output\UserSample\Debug_imguiEditor\"
xcopy /y /d "Libs\dll\Release\" "output\UserSample\Release\"
xcopy /y /d "Libs\dll\Release_imguiEditor\" "output\UserSample\Release_imguiEditor\"
xcopy /y /d "Libs\lib\Debug\" "output\UserSample\Debug\"
xcopy /y /d "Libs\lib\Debug_imguiEditor\" "output\UserSample\Debug_imguiEditor\"
xcopy /y /d "Libs\lib\Release\" "output\UserSample\Release\"
xcopy /y /d "Libs\lib\Release_imguiEditor\" "output\UserSample\Release_imguiEditor\"
