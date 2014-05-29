@echo off
set twsdir=%~dp0..\..
set blddir=%CD%

lupdate -no-obsolete "%twsdir%\netcoupler.pro"
qmake -config release "%twsdir%\netcoupler.pro" -spec win32-g++
mingw32-make -j4

xcopy /H /O /T /S /E /R /V "%twsdir%\bld\*" .\

copy %QTDIR%\QtCreator\bin\mingwm10.dll   .\
copy %QTDIR%\QtCreator\bin\libgcc*        .\
copy %QTDIR%\QtCreator\bin\QtCore4.dll    .\
copy %QTDIR%\QtCreator\bin\QtGui4.dll     .\
copy %QTDIR%\QtCreator\bin\QtNetwork4.dll .\
copy %QTDIR%\QtCreator\bin\phonon4.dll    .\
copy %QTDIR%\QtCreator\bin\QtSql4.dll     .\

mkdir iconengines
copy %QTDIR%\plugins\iconengines\qsvgicon4.dll .\iconengines\

mkdir imageformats
copy %QTDIR%\plugins\imageformats\qgif4.dll  .\imageformats
copy %QTDIR%\plugins\imageformats\qico4.dll  .\imageformats
copy %QTDIR%\plugins\imageformats\qjpeg4.dll .\imageformats
copy %QTDIR%\plugins\imageformats\qsvg4.dll  .\imageformats

mkdir phonon_backend
copy %QTDIR%\plugins\phonon_backend\phonon_ds94.dll .\phonon_backend

mkdir sqldrivers
copy %QTDIR%\plugins\sqldrivers\qsqlite4.dll .\sqldrivers

cd "%twsdir%\translations"
lrelease "%twsdir%\netcoupler.pro"
cd %blddir%
move "%twsdir%\translations\*" .\translations

del Makefile
del *.sym
del *.map

rem makensis.exe "setup win32.nsi"
echo *********************************************************
echo *** MAKING THE WINDOWS INSTALLER IS NOT SUPPORTED YET ***
echo *********************************************************
echo Finished.