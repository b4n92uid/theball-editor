# This file is generated automatically. Do not edit.
# Use project properties -> Build -> Qt -> Expert -> Custom Definitions.
TEMPLATE = app
DESTDIR = debug
TARGET = tbeditor
VERSION = 1.0.0
CONFIG -= debug_and_release app_bundle lib_bundle
CONFIG += debug 
QT = core gui opengl
SOURCES += sources/QVectorBox.cpp sources/main.cpp sources/QNodeBinders.cpp sources/QBrowsEdit.cpp sources/MainWindow.cpp sources/QTBEngine.cpp sources/QVector2Box.cpp
HEADERS += sources/QNodeBinders.h sources/QVector2Box.h sources/QBrowsEdit.h sources/MainWindow.h sources/QVectorBox.h sources/QTBEngine.h sources/Tools.h
FORMS += sources/about.ui sources/interface.ui
RESOURCES += sources/ressource.qrc
TRANSLATIONS +=
OBJECTS_DIR = build/Debug/MinGW-Windows
MOC_DIR = build
RCC_DIR = build
UI_DIR = sources
QMAKE_CC = gcc.exe
QMAKE_CXX = g++.exe
DEFINES += _DEBUG 
INCLUDEPATH += /D/projets/cpp/tbengine/sources 
LIBS += D:/projets/cpp/tbengine/dist/Debug/MinGW-Windows/libtbengine.a D:/librairies/cpp/devil/DevIL-SDK-x86-1.7.8/lib/DevIL.lib D:/librairies/cpp/freetype/freetype-2.3.9/objs/.libs/libfreetype.a -lopengl32 -lglu32  
CONFIG += console
RC_FILE += ../sources/ressource.rc
