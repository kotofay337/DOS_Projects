pklite tbx.exe
pklite tbl.exe
pklite mousetst.exe

mkdir tbg_rar
cd tbg_rar
mkdir bin
mkdir doc
mkdir include
mkdir lib
mkdir source
cd..

copy red.exe      tbg.exe
copy tbx.cfg      tbg_rar\bin\*.*
copy tb*.exe      tbg_rar\bin\*.*
copy   *.chr      tbg_rar\bin\*.*
copy   *.bgi      tbg_rar\bin\*.*
copy tb*.tbl      tbg_rar\bin\*.*
copy tbg_hlp.txt  tbg_rar\bin\*.*
copy mo*.exe      tbg_rar\bin\*.*
del  tbg.exe


copy tbg_prog.txt  tbg_rar\doc\*.*
copy tbg_user.txt  tbg_rar\doc\*.*
copy tbg_tc.h      tbg_rar\include\*.*
copy tbg_tcl.lib   tbg_rar\lib\*.*


copy t1.prj       tbg_rar\source\*.*
copy t2.prj       tbg_rar\source\*.*
copy tbg_tcl.prj  tbg_rar\source\*.*
copy t1.c         tbg_rar\source\*.*
copy tbg*.c       tbg_rar\source\*.*
copy tst*.tbg     tbg_rar\source\*.*

copy readme.      tbg_rar\*.*
copy info_tbg.txt tbg_rar\*.*
cd tbg_rar

del  tbgrar.exe
rar  a -sfx -s -r -m5 -xreadme. tbgrar.exe
md               b:\tbg_rar
copy tbgrar.exe  b:\tbg_rar
copy readme      b:\tbg_rar