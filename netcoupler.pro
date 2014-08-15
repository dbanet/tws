DEFINES += TWS_VERSION=\\\"3.0-alpha\\\"
BUILD_WITH_PHONON = NO

TEMPLATE += app
QT += gui network svg sql webkit
DEPENDPATH += . \    
    translations
INCLUDEPATH += .
MOC_DIR += .moc
UI_DIR += .ui
OBJECTS_DIR += .lib
RCC_DIR += .rcc

QMAKE_CXXFLAGS += -std=c++11 -Wall
# -O3
# -Werror

#     DEFINES += WITH_GAMESURGE_SUPPORT #  not ready yet
win32:DEFINES += WITH_WORMNAT_SUPPORT   # currently windows-only

!contains(QT_MAJOR_VERSION,5):!os2:!contains(BUILD_WITH_PHONON,NO){
    QT      += phonon
    DEFINES += PHONON
    HEADERS += src/sound_handler.h
    SOURCES += src/sound_handler.cpp
}

win32:LIBS += -lwsock32
# Input
HEADERS += src/about.h \
    src/awaybox.h \
    src/buddylist.h \
    src/buttonlayout.h \
    src/charformatsettings.h \
    src/chathandler.h \
    src/chathandlerprv.h \
    src/chatwindow.h \
    src/ctcphandler.h \
    src/ctcphandleratomic.h \
    src/ctctphandlerwidget.h \
    src/emoticonhandler.h \
    src/hostbox.h \
    src/hoststruct.h \
    src/inihandlerclass.h \
    src/ircnet.h \
    src/joinprvgame.h \
    src/logbrowser.h \
    src/maintoolbox.h \
    src/mytabwidget.h \
    src/netcoupler.h \
    src/settingswindow.h \
    src/snoppanet.h \
    src/snpsettings.h \
    src/textschemehandler.h \
    src/uihelper.h \
    src/usermodel.h \
    src/userstruct.h \
    src/volumeslider.h \
    src/global_macros.h \
    src/singleton.h \
    src/global_functions.h \
    src/combobox_wrapper.h \
    src/balloon_handler.h \    
    src/codecselectdia.h \
    src/clantowebpagemapper.h \
    src/mydebugclass.h \
    src/myDebug.h \
    src/leagueserverhandler.h \
    src/picturehandler.h \
    src/quithandler.h \
    src/mynetworkreply.h \
    src/qobjectwrapper.h \
    src/awayhandler.h \
    src/sqlsettings.h \
    src/settings.h \
    src/settingshelper.h \
    src/database_base.h \
    src/progressdialog.h \
    src/usermessage.h \
    src/emoticonsdialog.h \
    src/wa_textcodec.h \
    src/playername.h \
    src/ircmessage.h \
    src/mainwindow.h \
    src/channel/channeltab.h \
    src/server/servertab.h \
    src/channel/hostmodel.h \
    src/channel/chanusermodel.h

FORMS += src/about.ui \
    src/awaybox.ui \
    src/buddylist.ui \
    src/buttonlayout.ui \
    src/chatwindow.ui \
    src/ctcphandleratomic.ui \
    src/ctctphandlerwidget.ui \
    src/emoticonhandler.ui \
    src/filehistorylister.ui \
    src/hostbox.ui \
    src/joinprvgame.ui \
    src/logbrowser.ui \
    src/maintoolbox.ui \
    src/settingswindow.ui \
    src/textschemehandler.ui \
    src/volumeslider.ui \
    src/combobox_wrapper.ui \
    src/codecselectdia.ui \
    src/progressdialog.ui \
    src/emoticonsdialog.ui \
    src/playername.ui \
    src/mainwindow.ui \
    src/channel/channeltab.ui \
    src/channel/channelmenu.ui \
    src/server/servertab.ui \
    src/channel/chanUserListMenu.ui

SOURCES +=  src/about.cpp \
    src/awaybox.cpp \
    src/buddylist.cpp \
    src/buttonlayout.cpp \
    src/charformatsettings.cpp \
    src/chathandler.cpp \
    src/chathandlerprv.cpp \
    src/chatwindow.cpp \
    src/ctcphandler.cpp \
    src/ctcphandleratomic.cpp \
    src/ctctphandlerwidget.cpp \
    src/emoticonhandler.cpp \
    src/hostbox.cpp \
    src/hoststruct.cpp \
    src/inihandlerclass.cpp \
    src/ircnet.cpp \
    src/joinprvgame.cpp \
    src/logbrowser.cpp \
    src/main.cpp \
    src/maintoolbox.cpp \
    src/mytabwidget.cpp \
    src/netcoupler.cpp \
    src/settingswindow.cpp \
    src/snoppanet.cpp \
    src/snpsettings.cpp \
    src/textschemehandler.cpp \
    src/uihelper.cpp \
    src/usermodel.cpp \
    src/userstruct.cpp \
    src/volumeslider.cpp \
    src/combobox_wrapper.cpp \
    src/balloon_handler.cpp \
    src/codecselectdia.cpp \
    src/clantowebpagemapper.cpp \
    src/fakeObject.cpp \
    src/mydebugclass.cpp \
    src/myDebug.cpp \
    src/leagueserverhandler.cpp \
    src/global_functions.cpp \
    src/picturehandler.cpp \
    src/quithandler.cpp \
    src/mynetworkreply.cpp \
    src/awayhandler.cpp \
    src/sqlsettings.cpp \
    src/database_base.cpp \
    src/progressdialog.cpp \
    src/usermessage.cpp \
    src/emoticonsdialog.cpp \
    src/wa_textcodec.cpp \
    src/playername.cpp \
    src/ircmessage.cpp \
    src/mainwindow.cpp \
    src/channel/channeltab.cpp \
    src/server/servertab.cpp \
    src/channel/hostmodel.cpp \
    src/channel/chanusermodel.cpp

win32:RC_FILE += src/myapp.rc
TRANSLATIONS += \
    translations/The_Wheat_Snooper_untranslated.ts \
    translations/_da.Danish.ts \
    translations/_de.German_by_lookias.ts \
    translations/_es.Spanish_by_eViL.ts \
    translations/_fr.French_by_LeTotalKiller.ts \
    translations/_pt.Portugues_by_Kaleu.ts \
    translations/_ro.Romanian_by_MihaiS.ts \
    translations/_ru.Russian_translation_by_StepS.ts \
    translations/_pl.Polish_by_CzAjNiK.ts \
    translations/_en.Standard.ts \
    translations/es.Spanish_(Spain)_by_WowwoW.ts \
    translations/_lt.Lithuanian_by_K4Tsis.ts

RESOURCES += \
    res/pictureResource.qrc \
    res/about.qrc

OTHER_FILES += \
    res/about.html
