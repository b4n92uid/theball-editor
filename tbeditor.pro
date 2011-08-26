#-------------------------------------------------
#
# Project created by QtCreator 2011-04-04T20:35:38
#
#-------------------------------------------------

include(build/cache.pro)

QT += core gui opengl

TARGET = tbeditor
TEMPLATE = app

INCLUDEPATH += $$TBENGINE_INC $$DEVIL_INC $$FREETYPE_INC $$QUAZIP_INC $$ZLIB_INC

LIBS += $$TBENGINE_LIB $$DEVIL_LIB $$FREETYPE_LIB $$QUAZIP_LIB $$ZLIB_LIB -lopengl32 -lglu32

UI_DIR = ../sources

release {
    CONFIG += WINDOWS

    MOC_DIR     = ../build/release
    OBJECTS_DIR = ../build/release
    RCC_DIR     = ../build/release
    DESTDIR     = ../release
}

debug {
    CONFIG += CONSOLE
    DEFINES += _DEBUG

    MOC_DIR     = ../build/debug
    OBJECTS_DIR = ../build/debug
    RCC_DIR     = ../build/debug
    DESTDIR     = ../debug
}

SOURCES += \
    sources/MainWindow.cpp \
    sources/QVectorBox.cpp \
    sources/QVector2Box.cpp \
    sources/QTBEngine.cpp \
    sources/QNodeBinders.cpp \
    sources/QBrowsEdit.cpp \
    sources/QSignalBlocker.cpp \
    sources/PackerDialog.cpp \
    sources/main.cpp

HEADERS  += \
    sources/MainWindow.h \
    sources/Tools.h \
    sources/QVectorBox.h \
    sources/QVector2Box.h \
    sources/QTBEngine.h \
    sources/QNodeBinders.h \
    sources/QBrowsEdit.h \
    sources/QSignalBlocker.h \
    sources/PackerDialog.h \

FORMS += \
    sources/interface.ui

RESOURCES += \
    sources/ressource.qrc

RC_FILE += \
    sources/ressource.rc
