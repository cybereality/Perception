TEMPLATE = lib
CONFIG  += shared
CONFIG  -= app_bundle
CONFIG  -= qt

DESTDIR = "C:/Program Files (x86)/Vireio/Perception/bin"


LIBS += -ldbghelp  -lpsapi

SOURCES += \
    ../src/hijack_hook.cpp \
    ../src/hijack_funcs.cpp

HEADERS += \
    ../src/hijack_hook.h



