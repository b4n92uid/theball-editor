# This file is generated automatically. Do not edit.
# Use project properties -> Build -> Qt -> Expert -> Custom Definitions.
TEMPLATE = app
DESTDIR = release
TARGET = tbeditor
VERSION = 1.0.0
CONFIG -= debug_and_release app_bundle lib_bundle
CONFIG += release 
QT = core gui opengl
SOURCES += sources/QVectorBox.cpp sources/QSignalBlocker.cpp sources/main.cpp sources/QNodeBinders.cpp sources/QBrowsEdit.cpp sources/MainWindow.cpp sources/PackerDialog.cpp sources/QTBEngine.cpp sources/QVector2Box.cpp
HEADERS += sources/QVector2Box.h sources/QNodeBinders.h sources/QBrowsEdit.h sources/PackerDialog.h sources/MainWindow.h sources/QVectorBox.h sources/QTBEngine.h sources/Tools.h sources/QSignalBlocker.h
FORMS += sources/about.ui sources/packer.ui sources/interface.ui sources/matedit.ui
RESOURCES += sources/ressource.qrc
TRANSLATIONS +=
OBJECTS_DIR = build/Release/MinGW-Windows
MOC_DIR = build
RCC_DIR = build
UI_DIR = sources
QMAKE_CC = gcc.exe
QMAKE_CXX = g++.exe
DEFINES += 
INCLUDEPATH += /D/projets/cpp/tbengine/sources /D/librairies/cpp/qt/quazip-0.4.1/quazip /D/librairies/cpp/zlib/zlib123 
LIBS += D:/librairies/cpp/qt/quazip-0.4.1/quazip/release/libquazip.a D:/librairies/cpp/zlib/zlib123/libzlib.a D:/projets/cpp/tbengine/dist/Release/MinGW-Windows/libtbengine.a D:/librairies/cpp/devil/DevIL-SDK-x86-1.7.8/lib/DevIL.lib D:/librairies/cpp/freetype/freetype-2.3.9/objs/.libs/libfreetype.a -lopengl32 -lglu32  
RC_FILE += ../sources/ressource.rc
