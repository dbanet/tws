lupdate netcoupler.pro
qmake -config release
mingw32-make -j2 release
copy %QTDIR%\bin\mingwm10.dll "TheWheatSnooper"
copy %QTDIR%\bin\libgcc* "TheWheatSnooper"
copy %QTDIR%\bin\QtCore4.dll "TheWheatSnooper"
copy %QTDIR%\bin\QtGui4.dll "TheWheatSnooper"
copy %QTDIR%\bin\QtNetwork4.dll "TheWheatSnooper"
copy %QTDIR%\bin\phonon4.dll "TheWheatSnooper"
lrelease netcoupler.pro
copy /Y translations\* "TheWheatSnooper\translations"
makensis.exe "setup win32.nsi"