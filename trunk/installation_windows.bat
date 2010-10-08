lupdate -no-obsolete netcoupler.pro
qmake -config release
mingw32-make -j2 release
copy %QTDIR%\bin\mingwm10.dll "TheWheatSnooper"
copy %QTDIR%\bin\libgcc* "TheWheatSnooper"
copy %QTDIR%\bin\QtCore4.dll "TheWheatSnooper"
copy %QTDIR%\bin\QtGui4.dll "TheWheatSnooper"
copy %QTDIR%\bin\QtNetwork4.dll "TheWheatSnooper"
copy %QTDIR%\bin\phonon4.dll "TheWheatSnooper"

mkdir TheWheatSnooper\iconengines
copy %QTDIR%\plugins\iconengines\qsvgicon4.dll "TheWheatSnooper\iconengines"

mkdir TheWheatSnooper\imageformats
copy %QTDIR%\plugins\imageformats\qgif4.dll "TheWheatSnooper\imageformats"
copy %QTDIR%\plugins\imageformats\qico4.dll "TheWheatSnooper\imageformats"
copy %QTDIR%\plugins\imageformats\qjpeg4.dll "TheWheatSnooper\imageformats"
copy %QTDIR%\plugins\imageformats\qsvg4.dll "TheWheatSnooper\imageformats"

mkdir TheWheatSnooper\phonon_backend
copy %QTDIR%\plugins\phonon_backend\phonon_ds94.dll "TheWheatSnooper\phonon_backend"

cd translations
lrelease ..\netcoupler.pro
cd ..
copy /Y translations\* "TheWheatSnooper\translations"
makensis.exe "setup win32.nsi"