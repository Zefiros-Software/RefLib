@echo off
cd ./bin
doxygen ../documentation.doxy -u
hhc.exe ../out/html/index.hhp 
cd "../out/html/"
move "Reflection Reference.*" "../../"