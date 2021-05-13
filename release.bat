del "sources.7z"
mkdir setup
7z.exe a sources.7z sources
cd bin
windeployqt --release Tamise.exe
cd ..
Compil32.exe  /cc "release.iss"
pause