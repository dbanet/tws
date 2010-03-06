lupdate netcoupler.pro
qmake
mingw32-make release
move /Y Snooper.exe "The Wheat Snooper/The Wheat Snooper.exe"
copy %QTDIR%\bin\mingwm10.dll "The Wheat Snooper"
copy %QTDIR%\bin\libgcc* "The Wheat Snooper"
copy %QTDIR%\bin\QtCore4.dll "The Wheat Snooper"
copy %QTDIR%\bin\QtGui4.dll "The Wheat Snooper"
copy %QTDIR%\bin\QtNetwork4.dll "The Wheat Snooper"
copy %QTDIR%\bin\phonon4.dll "The Wheat Snooper"
lrelease netcoupler.pro
copy /Y translations\*.qm "The Wheat Snooper\translations"
del Snooper.exe