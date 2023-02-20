QT       += core gui multimedia sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    src/PopupWidget/Popup_playlist.cpp \
    src/mainPlayWidget.cpp \
    src/main_listWidget.cpp \
    src/main_navigation.cpp \
    src/mainwindow.cpp \
    src/music_playwidget.cpp \
    src/stackWidget/stack_findMusic.cpp \
    src/stackWidget/stack_foundMusic.cpp \
    src/stackWidget/stack_localMusic.cpp \
    src/stackWidget/stack_video.cpp

HEADERS += \
    lib/PopupWidget/Popup_playlist.h \
    lib/mainPlayWidget.h \
    lib/main_listWidget.h \
    lib/main_navigation.h \
    lib/mainwindow.h \
    lib/music_playwidget.h \
    lib/stackWidget/stack_findMusic.h \
    lib/stackWidget/stack_foundMusic.h \
    lib/stackWidget/stack_localMusic.h \
    lib/stackWidget/stack_video.h

INCLUDEPATH +=$$PWD Event
include (Event/PsEvent.pri)

INCLUDEPATH +=$$PWD NetWork
include (NetWork/NetWork.pri)

INCLUDEPATH +=$$PWD Database
include (Database/DataBase.pri)

INCLUDEPATH +=$$PWD ThirdParty
include (ThirdParty/ThirdParty.pri)




# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

FORMS += \
    ui/picturewidget.ui

RESOURCES += \
    res.qrc

DISTFILES +=
