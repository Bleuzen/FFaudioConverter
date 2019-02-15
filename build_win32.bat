SET PATH=%PATH%;C:\Qt\5.12.1\mingw73_64\bin;C:\Qt\Tools\mingw730_64\bin

mkdir build
cd build

qmake.exe ..\FFaudioConverter.pro -spec win32-g++ CONFIG+=release
mingw32-make.exe -j4

cd release

for /f "delims=" %%F in ('dir /b /a-d ^| findstr /vile ".exe"') do del "%%F"

windeployqt.exe --release --compiler-runtime .

rmdir /s /q iconengines imageformats styles
del D3Dcompiler_47.dll Qt5Svg.dll opengl32sw.dll libGLESV2.dll libEGL.dll

echo .
echo Build done. Now download FFmpeg and place it in the applications directory.
echo https://ffmpeg.zeranoe.com/builds/
