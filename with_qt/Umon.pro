#-------------------------------------------------
#
# Project created by QtCreator 2017-12-06T18:47:53
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Umon
TEMPLATE = app

CONFIG += c++14

Debug:DEFINES += _GLIBCXX_DEBUG=1

QMAKE_CXXFLAGS += -isystem $$[QT_INSTALL_HEADERS]

QMAKE_CXXFLAGS += \
    -Werror \
    -Wconversion \
    -Wall \
    -Wextra \
    -Wpedantic \
    -Wnon-virtual-dtor \
    -Wuninitialized \
    -Wmaybe-uninitialized \
    -Wcast-align \
    -Wunused \
    -Wno-sign-conversion \
    -Winit-self \
    -Wformat=2 \
    -Wlogical-op \
    -Wold-style-cast \

#    -Wshadow (ui) \
#    -Wuseless-cast (ordered_hash) \
#    -Wnull-dereference \
#    -Wduplicated-cond \
#    -Wduplicated-branches \
#    -Wlifetime \
#    -Wbool-conversions

win32-g++ {
   QMAKE_CXXFLAGS += -Werror -Wconversion -Wextra -Wpedantic
}
win32-msvc*{
   QMAKE_CXXFLAGS += /WX
}

INCLUDEPATH += qtsource
INCLUDEPATH += ../portable/app/main
INCLUDEPATH += ../portable/app/storage
INCLUDEPATH += ../portable/app/streaming
INCLUDEPATH += ../portable/app/ui/controls
INCLUDEPATH += ../portable/app/ui/drawing
INCLUDEPATH += ../portable/app/ui/view
INCLUDEPATH += ../portable/tests
INCLUDEPATH += ../portable/uiframework
INCLUDEPATH += ../portable/useful

SOURCES += \
    $$files(qtsource/*.cpp, true) \
    $$files(../portable/app/main/*.cpp, true) \
    $$files(../portable/app/storage/*.cpp, true) \
    $$files(../portable/app/streaming/*.cpp, true) \
    $$files(../portable/app/ui/controls/*.cpp, true) \
    $$files(../portable/app/ui/drawing/*.cpp, true) \
    $$files(../portable/app/ui/view/*.cpp, true) \
    $$files(../portable/tests/*.cpp, true) \
    $$files(../portable/uiframework/*.cpp, true) \
    $$files(../portable/useful/*.cpp, true) \

HEADERS  += \
    $$files(qtsource/*.h, true) \
    $$files(../portable/app/main/*.h, true) \
    $$files(../portable/app/storage/*.h, true) \
    $$files(../portable/app/streaming/*.h, true) \
    $$files(../portable/app/ui/controls/*.h, true) \
    $$files(../portable/app/ui/drawing/*.h, true) \
    $$files(../portable/app/ui/view/*.h, true) \
    $$files(../portable/tests/*.h, true) \
    $$files(../portable/uiframework/*.h, true) \
    $$files(../portable/useful/*.h, true) \

FORMS    +=
