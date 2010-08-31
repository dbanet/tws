TEMPLATE = app
TARGET = 
QT += gui \
    network \
    svg \
    phonon
DEPENDPATH += . \
    qfileupload \
    translations
INCLUDEPATH += . \
    qfileupload
MOC_DIR = .moc
UI_DIR = .ui
OBJECTS_DIR = .lib
RCC_DIR = .rcc
unix:TARGET = "./TheWheatSnooper/TheWheatSnooper"
win32:TARGET = "../TheWheatSnooper/TheWheatSnooper"
unix:DEFINES += QT_WS_X11
CONFIG += debug_and_release

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
    src/hostmodel.h \
    src/hostprvbox.h \
    src/hoststruct.h \
    src/inihandlerclass.h \
    src/initextbrowser.h \
    src/ircnet.h \
    src/joinprvgame.h \
    src/logbrowser.h \
    src/maintoolbox.h \
    src/mainwindow.h \
    src/mytabwidget.h \
    src/netcoupler.h \
    src/playername.h \
    src/settingswindow.h \
    src/snoppanet.h \
    src/snpsettings.h \
    src/textschemehandler.h \
    src/uihelper.h \
    src/uploadedfileatomic.h \
    src/usermodel.h \
    src/userstruct.h \
    src/volumeslider.h \
    src/window.h \
    src/global_macros.h \
    src/singleton.h \
    src/sound_handler.h \
    src/global_functions.h \
    src/combobox_wrapper.h \
    src/balloon_handler.h
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
    src/hostprvbox.ui \
    src/initextbrowser.ui \
    src/joinprvgame.ui \
    src/logbrowser.ui \
    src/maintoolbox.ui \
    src/mainwindow.ui \
    src/playername.ui \
    src/settingswindow.ui \
    src/textschemehandler.ui \
    src/uploadedfileatomic.ui \
    src/volumeslider.ui \
    src/window.ui \
    src/window2.ui \
    src/window3.ui \
    src/combobox_wrapper.ui
SOURCES +=   src/about.cpp \
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
    src/hostmodel.cpp \
    src/hostprvbox.cpp \
    src/hoststruct.cpp \
    src/inihandlerclass.cpp \
    src/initextbrowser.cpp \
    src/ircnet.cpp \
    src/joinprvgame.cpp \
    src/logbrowser.cpp \
    src/main.cpp \
    src/maintoolbox.cpp \
    src/mainwindow.cpp \
    src/mytabwidget.cpp \
    src/netcoupler.cpp \
    src/playername.cpp \
    src/settingswindow.cpp \
    src/snoppanet.cpp \
    src/snpsettings.cpp \
    src/textschemehandler.cpp \
    src/uihelper.cpp \
    src/uploadedfileatomic.cpp \
    src/usermodel.cpp \
    src/userstruct.cpp \
    src/volumeslider.cpp \
    src/window.cpp \   
    src/sound_handler.cpp \
    src/combobox_wrapper.cpp \
    src/balloon_handler.cpp
RC_FILE = src/myapp.rc
TRANSLATIONS += translations/Romanian_translation_The_Wheat_Snooper_by_MihaiS.ts \
    translations/The_Wheat_Snooper_danish.ts \
    translations/The_Wheat_Snooper_german_by_lookias.ts \
    translations/The_Wheat_Snooper_international_spanish.ts \
    translations/The_Wheat_Snooper_portugues.ts \
    translations/The_Wheat_Snooper_spanish-(of_spain)_by_kosmos.ts \
    translations/The_Wheat_Snooper_untranslated.ts \
    translations/The_Wheat_Snooper_spanish-(of_spain)_by_kosmos.ts \
    translations/Russian_translation_by_StepS.ts
