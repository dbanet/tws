/**/
parse source . . scrpath
twsdir=filespec('drive',scrpath)||filespec('path',scrpath)||'..\..'
blddir=directory()
qtdir=value('qtdir',,os2environment)
if qtdir='' then qtdir='\usr\lib\qt4'

'"'qtdir'\bin\lupdate" -no-obsolete "'twsdir'\netcoupler.pro"'
'qmake "'twsdir'\netcoupler.pro"'
'make -j4'

'xcopy /H /O /T /S /E /R /V "'twsdir'\bld\*" .\'

'cd "'twsdir'\translations"'
'"'qtdir'\bin\lrelease" "'twsdir'\netcoupler.pro"'
'cd "'blddir'"'
'move "'twsdir'\translations\*.qm" .\translations'

say 'Finished.'