pklite tbx.exe
pklite tbl.exe

mkdir tbg_sml
cd tbg_sml
mkdir bin
mkdir doc
mkdir include
mkdir lib
cd..                   

copy red.exe      tbg.exe
copy tbx.cfg      tbg_sml\bin\*.*
copy tb*.exe      tbg_sml\bin\*.*
copy   *.chr      tbg_sml\bin\*.*
copy   *.bgi      tbg_sml\bin\*.*
copy tb*.tbl      tbg_sml\bin\*.*
copy tbg_hlp.txt  tbg_sml\bin\*.*
del  tbg.exe


copy tbg_prog.txt  tbg_sml\doc\*.*
copy tbg_user.txt  tbg_sml\doc\*.*
copy tbg_tc.h      tbg_sml\include\*.*
copy tbg_tcl.lib   tbg_sml\lib\*.*

cd tbg_sml

md               b:\tbg_sml
md               b:\tbg_sml\bin
md               b:\tbg_sml\doc
md               b:\tbg_sml\include
md               b:\tbg_sml\lib
cd bin
copy *.*         b:\tbg_sml\bin
cd..
cd doc
copy *.*         b:\tbg_sml\doc
cd..
cd include
copy *.*         b:\tbg_sml\include
cd..
cd lib
copy *.*         b:\tbg_sml\lib
cd..
