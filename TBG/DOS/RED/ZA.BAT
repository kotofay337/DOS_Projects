@echo off
@del zred5rar.exe
@rar a -sfx -s -m5 -x@z.lst zred5rar
@md                a:\red5_src
@copy zred5rar.exe a:\red5_src
@echo Вставьте следующий диск и
@pause
@copy zred5rar.exe a:\red5_src
