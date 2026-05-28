rem Batch-file to make Borlans C Versions of the CTask kernel
rem and support routines.
tmake -fctask.bc
if errorlevel 1 goto exit
tmake -fctsup.bc
:exit
