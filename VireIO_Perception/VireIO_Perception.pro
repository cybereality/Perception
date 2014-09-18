TEMPLATE = app
QT      += core gui widgets xml
CONFIG  += c++11
DEFINES -= UNICODE


DESTDIR = "C:/Program Files (x86)/Vireio/Perception/bin"


INCLUDEPATH +=            \
    ../DxProxy            \
    ../VRboostReferee     \
    $$(DXSDK_DIR)/Include



SOURCES += \
    ../DxProxy/cStereoMode.cpp \
    hijack.cpp \
    cGameProfile.cpp \
    cMainWindow.cpp \
    cGame.cpp \
    loader.cpp \
    app_main.cpp

HEADERS  += \
    cMainWindow.h \
    ../DxProxy/cStereoMode.h \
    hijack.h \
    cGameProfile.h \
    cGame.h \
    loader.h

FORMS    += \
    cMainWindow.ui
