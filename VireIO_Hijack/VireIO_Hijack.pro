TEMPLATE = lib
CONFIG  += shared
CONFIG  -= app_bundle
CONFIG  -= qt

DESTDIR = "C:/Program Files (x86)/Vireio/Perception/bin"


LIBS += -ldbghelp  -lpsapi

SOURCES += \
    hijack_hook.cpp \
    hijack_funcs.cpp

HEADERS += \
    hijack_hook.h



