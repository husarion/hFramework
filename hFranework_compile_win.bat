echo off
mkdir build\stm32_core2_1.0.0\
cd build\stm32_core2_1.0.0\
set PATH=C:\Users\%username%\.vscode\HusarionTools\bin;%PATH%
cmake ../.. -DBOARD_TYPE=core2 -DPORT=stm32 -DHFRAMEWORK_PATH=. -GNinja
ninja
echo Compilation done
pause