rem Batch file to make all sample applications for Turbo C
tmake %1 %2 %3 %4 %5 -fres.bc
if errorlevel 1 goto exit
tmake %1 %2 %3 %4 %5 -fminres.bc
if errorlevel 1 goto exit
tmake %1 %2 %3 %4 %5 -fsnap.bc
if errorlevel 1 goto exit
tmake %1 %2 %3 %4 %5 -fspawn.bc
if errorlevel 1 goto exit
tmake %1 %2 %3 %4 %5 -fmspawn.bc
if errorlevel 1 goto exit
tmake %1 %2 %3 %4 %5 -ftest.bc
if errorlevel 1 goto exit
tmake %1 %2 %3 %4 %5 -ftprt.bc
if errorlevel 1 goto exit
tmake %1 %2 %3 %4 %5 -ftsio.bc
if errorlevel 1 goto exit
tmake %1 %2 %3 %4 %5 -ftestmod.bc
:exit
