#-------------------------------------------------
#
# Project created by QtCreator 2011-04-04T20:35:38
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = tbeditor
TEMPLATE = app

INCLUDEPATH += D:\projets\cpp\tbengine\sources

LIBS += D:/librairies/cpp/devil/DevIL-SDK-x86-1.7.8/lib/DevIL.lib \
        D:/librairies/cpp/freetype/freetype-2.3.9/objs/.libs/libfreetype.a \
        -lopengl32 \
        -lglu32

UI_DIR = sources

debug {
    CONFIG += console
    DEFINES += _DEBUG

    MOC_DIR = build/debug
    OBJECTS_DIR = build/debug
    RCC_DIR = build/debug

    LIBS += -LD:\projets\cpp\tbengine\dist\Debug\MinGW-Windows\libtbengine.a
}

release {
    MOC_DIR = build/release
    OBJECTS_DIR = build/release
    RCC_DIR = build/release

    LIBS += -LD:\projets\cpp\tbengine\dist\Release\MinGW-Windows\libtbengine.a
}

SOURCES += sources/MainWindow.cpp \
    sources/QVectorBox.cpp \
    sources/QVector2Box.cpp \
    sources/QTBEngine.cpp \
    sources/QNodeBinders.cpp \
    sources/QBrowsEdit.cpp \
    sources/main.cpp

HEADERS  += sources/MainWindow.h \
    sources/Tools.h \
    sources/QVectorBox.h \
    sources/QVector2Box.h \
    sources/QTBEngine.h \
    sources/QNodeBinders.h \
    sources/QBrowsEdit.h \

FORMS += \
    sources/interface.ui

RESOURCES += \
    sources/ressource.qrc

RC_FILE += sources/ressource.rc
