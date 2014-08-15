#######################################
#      _________________________      #
#     /__   _\ \  /  \  / / ___/      #
#        | |  \ \/ /\ \/ /\___        #
#        | |   \  /  \  /___  \       #
#        |_|____\/____\/______/       #
#                                     #
#######################################

SET (PROJECT netcoupler)

CMAKE_MINIMUM_REQUIRED (VERSION 2.8)
CMAKE_POLICY (VERSION 2.8)
IF (CMAKE_MAJOR_VERSION GREATER 2)
    CMAKE_POLICY (SET CMP0037 OLD)
ENDIF ()

ADD_DEFINITIONS (-DTWS_VERSION="3.0-alpha")
PROJECT (${PROJECT})

#######################################
##           FILES SECTION           ##
#######################################

# the base name of the main binary (executable) file
SET (BINARY_NAME ${PROJECT})

# where the snooper will be built to, and
# where the files from ${BUILD_ROOT_SOURCE_DIR}
# will be copied to. Located in ${CMAKE_BINARY_DIR}
SET (BUILD_ROOT
    out)

# this directory's contents will be copied to the snooper's
# build root. Files that the program will need at runtime
# should be placed there. Located in ${CMAKE_SOURCE_DIR}
SET (BUILD_ROOT_SOURCE_DIR
    build-files)

SET (HEADERS 
    src/about.h
    src/awaybox.h 
    src/buddylist.h
    src/buttonlayout.h
    #src/charformatsettings.h
    src/chathandler.h
    src/chathandlerprv.h
    src/chatwindow.h
    src/ctcphandler.h
    src/ctcphandleratomic.h
    src/ctctphandlerwidget.h
    src/emoticonhandler.h
    src/hostbox.h
    #src/hoststruct.h
    #src/inihandlerclass.h
    src/ircnet.h
    src/joinprvgame.h
    src/logbrowser.h
    src/maintoolbox.h
    src/mytabwidget.h
    src/netcoupler.h
    src/settingswindow.h
    src/snoppanet.h
    #src/snpsettings.h
    src/textschemehandler.h
    #src/uihelper.h
    src/usermodel.h
    #src/userstruct.h
    src/volumeslider.h
    #src/global_macros.h
    #src/singleton.h
    #src/global_functions.h
    src/combobox_wrapper.h
    src/balloon_handler.h    
    src/codecselectdia.h
    src/clantowebpagemapper.h
    #src/mydebugclass.h
    #src/myDebug.h
    src/leagueserverhandler.h
    #src/picturehandler.h
    src/quithandler.h
    src/mynetworkreply.h
    #src/qobjectwrapper.h
    src/awayhandler.h
    #src/sqlsettings.h
    #src/settings.h
    #src/settingshelper.h
    #src/database_base.h
    src/progressdialog.h
    #src/usermessage.h
    src/emoticonsdialog.h
    #src/wa_textcodec.h
    src/playername.h
    #src/ircmessage.h
    src/mainwindow.h
    src/channel/channeltab.h
    src/server/servertab.h
    src/channel/hostmodel.h
    src/channel/chanusermodel.h)

SET (SOURCES
    src/about.cpp
    src/awaybox.cpp
    src/buddylist.cpp
    src/buttonlayout.cpp
    src/charformatsettings.cpp
    src/chathandler.cpp
    src/chathandlerprv.cpp
    src/chatwindow.cpp
    src/ctcphandler.cpp
    src/ctcphandleratomic.cpp
    src/ctctphandlerwidget.cpp
    src/emoticonhandler.cpp
    src/hostbox.cpp
    src/hoststruct.cpp
    src/inihandlerclass.cpp
    src/ircnet.cpp
    src/joinprvgame.cpp
    src/logbrowser.cpp
    src/main.cpp
    src/maintoolbox.cpp
    src/mytabwidget.cpp
    src/netcoupler.cpp
    src/settingswindow.cpp
    src/snoppanet.cpp
    src/snpsettings.cpp
    src/textschemehandler.cpp
    src/uihelper.cpp
    src/usermodel.cpp
    src/userstruct.cpp
    src/volumeslider.cpp
    src/combobox_wrapper.cpp
    src/balloon_handler.cpp
    src/codecselectdia.cpp
    src/clantowebpagemapper.cpp
    src/fakeObject.cpp
    src/mydebugclass.cpp
    src/myDebug.cpp
    src/leagueserverhandler.cpp
    src/global_functions.cpp
    src/picturehandler.cpp
    src/quithandler.cpp
    src/mynetworkreply.cpp
    src/awayhandler.cpp
    src/sqlsettings.cpp
    src/database_base.cpp
    src/progressdialog.cpp
    src/usermessage.cpp
    src/emoticonsdialog.cpp
    src/wa_textcodec.cpp
    src/playername.cpp
    src/ircmessage.cpp
    src/mainwindow.cpp
    src/channel/channeltab.cpp
    src/server/servertab.cpp
    src/channel/hostmodel.cpp
    src/channel/chanusermodel.cpp)

SET (FORMS
    src/about.ui
    src/awaybox.ui
    src/buddylist.ui
    src/buttonlayout.ui
    src/chatwindow.ui
    src/ctcphandleratomic.ui
    src/ctctphandlerwidget.ui
    src/emoticonhandler.ui
    src/filehistorylister.ui
    src/hostbox.ui
    src/joinprvgame.ui
    src/logbrowser.ui
    src/maintoolbox.ui
    src/settingswindow.ui
    src/textschemehandler.ui
    src/volumeslider.ui
    src/combobox_wrapper.ui
    src/codecselectdia.ui
    src/progressdialog.ui
    src/emoticonsdialog.ui
    src/playername.ui
    src/mainwindow.ui
    src/channel/channeltab.ui
    src/channel/channelmenu.ui
    src/server/servertab.ui
    src/channel/chanUserListMenu.ui)

SET (RESOURCE_PATH ${CMAKE_SOURCE_DIR}/res)
SET (RESOURCES
    ${RESOURCE_PATH}/pictureResource.qrc
    ${RESOURCE_PATH}/about.qrc)

SET (LIBRARIES
    )

#######################################
##         UTILITY FUNCTIONS         ##
#######################################
FUNCTION (APPEND VAR)
    SET (${VAR} ${${VAR}} ${ARGN} PARENT_SCOPE)
ENDFUNCTION (APPEND)

#######################################
##         Qt / MISCELLANEOUS        ##
#######################################

# including Qt itself
FIND_PACKAGE (Qt4 REQUIRED)

# including Qt modules we need
SET (QT_USE_QTNETWORK TRUE)
SET (QT_USE_QTSVG TRUE)
SET (QT_USE_QTSQL TRUE)
SET (QT_USE_QTWEBKIT TRUE)

# in order to build a Qt application, the Qt include directories
# needs to be added as well as a range of defines need to be set
INCLUDE (${QT_USE_FILE})
ADD_DEFINITIONS (${QT_DEFINITIONS})

IF (NOT OS2 AND NOT BUILD_WITH_PHONON)
    # turning on sound support
    SET (QT_USE_PHONON TRUE)
    ADD_DEFINITIONS (-DPHONON)
    FIND_LIBRARY (PHONON_LIBRARY phonon4)
    APPEND (LIBRARIES ${PHONON_LIBRARY})

    SET (HEADERS ${HEADERS} src/sound_handler.h)
    SET (SOURCES ${SOURCES} src/sound_handler.cpp)
ENDIF ()

# Qt code generators processing
QT4_WRAP_UI (MOC_FORMS ${FORMS})
QT4_WRAP_CPP (MOC_SOURCES ${HEADERS})
QT4_ADD_RESOURCES (QRC_SOURCES ${RESOURCES})

# before this file can be used to build the project there is a
# small caveat to handle. As all intermediate files are generated
# outside the source tree, the header file generated by uic will not
# be located by the compiler. In order to handle this, the build
# directory needs to be added to the list of include directories.
INCLUDE_DIRECTORIES (${CMAKE_BINARY_DIR})

# turning on C++11 standart support, activating more warnings
ADD_COMPILE_OPTIONS (-std=c++11 -Wall)

IF (OS2)
    # for OS/2, turning on highmem support
    ADD_COMPILE_OPTIONS (-Zhigh-mem)
ENDIF ()

IF (WIN32)
    # for Windows, turning on WormNAT2 support 
    APPEND (LIBRARIES wsock32)
    ADD_DEFINITIONS(-DWITH_WORMNAT_SUPPORT)
ENDIF ()

#######################################
##        LANGUAGE PROCSESING        ##
#######################################

SET (TS_PATH ${CMAKE_SOURCE_DIR}/translations)
SET (QM_PATH ${CMAKE_BINARY_DIR}/${BUILD_ROOT}/translations)
SET (LANGUAGES
    danish
    french
    german
    lithuanian
    polish
    portuguese
    romanian
    russian
    spanish)

ADD_CUSTOM_TARGET (
    translations
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${TS_PATH} ${QM_PATH})

FOREACH (LANGUAGE ${LANGUAGES})
    APPEND (TSS ${QM_PATH}/${LANGUAGE}.ts)
ENDFOREACH ()

ADD_CUSTOM_COMMAND (
    TARGET translations 
    COMMAND ${QT_LUPDATE_EXECUTABLE} ${HEADERS} ${SOURCES} -ts ${TSS}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

ADD_CUSTOM_COMMAND (
    TARGET translations
    COMMAND ${QT_LRELEASE_EXECUTABLE} ${TSS})

ADD_CUSTOM_COMMAND (
    TARGET translations
    COMMAND ${CMAKE_COMMAND} -E remove ${QM_PATH}/.gitkeep ${TSS})

# must have build directories created to release translations to
ADD_DEPENDENCIES (translations buildtree)

#######################################
##        PREPARE BUILD TREE         ##
#######################################

ADD_CUSTOM_TARGET (
    buildtree
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/${BUILD_ROOT})

ADD_CUSTOM_COMMAND (
    TARGET buildtree
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/${BUILD_ROOT_SOURCE_DIR} ${CMAKE_BINARY_DIR}/${BUILD_ROOT})

#######################################
##             BUILDING              ##
#######################################

ADD_EXECUTABLE (${BUILD_ROOT}/${BINARY_NAME} ${HEADERS} ${SOURCES} ${MOC_FORMS} ${MOC_SOURCES} ${QRC_SOURCES})
ADD_DEPENDENCIES (${BUILD_ROOT}/${BINARY_NAME} buildtree translations)
TARGET_LINK_LIBRARIES (${BUILD_ROOT}/${BINARY_NAME} ${LIBRARIES} ${QT_LIBRARIES})
