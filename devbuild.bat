del "sources.7z"
mkdir setup
start "" "C:\Program Files\7-Zip\7z.exe" a sources.7z sources
start "" "C:\Program Files (x86)\Inno Setup 5\Compil32.exe" /cc "devbuild.iss"