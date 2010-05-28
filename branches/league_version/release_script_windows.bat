lupdate netcoupler.pro
qmake
mingw32-make release
copy %QTDIR%\bin\mingwm10.dll "TheWheatSnooper"
copy %QTDIR%\bin\libgcc* "TheWheatSnooper"
copy %QTDIR%\bin\QtCore4.dll "TheWheatSnooper"
copy %QTDIR%\bin\QtGui4.dll "TheWheatSnooper"
copy %QTDIR%\bin\QtNetwork4.dll "TheWheatSnooper"
copy %QTDIR%\bin\phonon4.dll "TheWheatSnooper"
lrelease netcoupler.pro
copy /Y translations\*.qm "TheWheatSnooper\translations"