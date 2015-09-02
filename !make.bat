@del /s vulnerable.obj vulnerable.exe
@lcc.exe -g2 vulnerable.c
@lcclnk.exe vulnerable.obj ws2_32.lib

