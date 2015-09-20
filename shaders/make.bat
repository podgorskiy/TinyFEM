call ..\tools\shadercDebug.exe -f src/vs_meshPlot.sc -i src/ --platform asm.js --bin2c -o vs_meshPlot.h --type vertex
call ..\tools\shadercDebug.exe -f src/fs_meshPlot.sc -i src/ --platform asm.js --bin2c -o fs_meshPlot.h --type fragment
call ..\tools\shadercDebug.exe -f src/vs_sign.sc -i src/ --platform asm.js --bin2c -o vs_sign.h --type vertex
call ..\tools\shadercDebug.exe -f src/fs_sign.sc -i src/ --platform asm.js --bin2c -o fs_sign.h --type fragment