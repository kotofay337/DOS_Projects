@Echo Off
@if "%1" == "" goto Err
@Echo Копируем все файлы...
@SET DP=%1\Parking
@mkdir %DP%                        > NUL
@mkdir %DP%\TBG                    > NUL
@mkdir %DP%\DB                     > NUL
@mkdir %DP%\SPOOL                  > NUL
@copy .\DB\CLR\*.dbf      %DP%\DB  > NUL
@copy .\DB\CLR\prndbf.exe %DP%\DB  > NUL
@copy .\TBG\*.tbg         %DP%\TBG > NUL
@copy .\sh.exe            %DP%     > NUL
@copy .\rep?.exe          %DP%     > NUL
@copy .\eventprn.exe      %DP%     > NUL
@copy .\ed.com            %DP%     > NUL
@copy .\ShConfig.txt      %DP%     > NUL
@copy .\macrotbm.txt      %DP%     > NUL
@Goto Ok
:Err
@Echo Укажите диск, например:
@Echo "A:\>install.bat C:"
goto Ext
:Ok
@Echo Готово!
:Ext
