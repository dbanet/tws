@echo off
set twsdir=%~dp0..
set blddir=%CD%

lupdate -no-obsolete "%twsdir%\netcoupler.pro"
qmake "%twsdir%\netcoupler.pro"
mingw32-make -j4

xcopy /H /O /T /S /E /R /V "%twsdir%\bld\*" .\

cd "%twsdir%\translations"
lrelease "%twsdir%\netcoupler.pro"
cd %blddir%
move "%twsdir%\translations\*" .\translations

echo Finished.