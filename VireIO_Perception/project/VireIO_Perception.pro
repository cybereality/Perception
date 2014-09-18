TEMPLATE = app
QT      += core gui widgets xml
CONFIG  += c++11
DEFINES -= UNICODE


DESTDIR = "C:/Program Files (x86)/Vireio/Perception/bin"


INCLUDEPATH += \
    ../../common

SOURCES += \
    ../src/cMainWindow.cpp \
    ../src/app_main.cpp \
    ../../common/cGame.cpp \
    ../../common/cGameProfile.cpp \
    ../../common/cPropsFile.cpp \
    ../../common/cSettings.cpp \
    ../../common/cStereoMode.cpp \
    ../../common/hijack.cpp \
    ../../common/cUser.cpp

HEADERS  += \
    ../src/cMainWindow.h \
    ../../common/cGame.h \
    ../../common/cGameProfile.h \
    ../../common/cPropsFile.h \
    ../../common/cSettings.h \
    ../../common/cStereoMode.h \
    ../../common/hijack.h \
    ../../common/VireIO.h \
    ../../common/cUser.h

FORMS    += \
    ../src/cMainWindow.ui
