del "sources.7z"
del /S /Q deploy
rmdir /S /Q deploy
mkdir setup
mkdir deploy
7z.exe a sources.7z sources
cd deploy
copy ..\bin\fmodex.dll
copy ..\build-Tamise-Desktop_Qt_5_15_2_MinGW_32_bit-Release\release\Tamise.exe
windeployqt --release Tamise.exe
cd ..
Compil32.exe  /cc "release.iss"
del "sources.7z"
rem del /S /Q deploy
rem rmdir /S /Q deploy