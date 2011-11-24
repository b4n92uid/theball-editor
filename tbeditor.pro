#-------------------------------------------------
#
# Project created by QtCreator 2011-04-04T20:35:38
#
#-------------------------------------------------


QT += core gui opengl

TARGET = tbeditor
TEMPLATE = app

CONFIG(release, debug|release) {
    
    include(build/cache.release)
    
    CONFIG += release
    CONFIG += WINDOWS

    MOC_DIR     = ../build/release
    OBJECTS_DIR = ../build/release
    RCC_DIR     = ../build/release
    DESTDIR     = ../release
}

CONFIG(debug, debug|release) {

    include(build/cache.debug)
    
    CONFIG += debug
    CONFIG += CONSOLE

    DEFINES += _DEBUG

    MOC_DIR     = ../build/debug
    OBJECTS_DIR = ../build/debug
    RCC_DIR     = ../build/debug
    DESTDIR     = ../debug
}

INCLUDEPATH += $$TBENGINE_INC $$DEVIL_INC $$FREETYPE_INC $$QUAZIP_INC $$ZLIB_INC

LIBS += $$TBENGINE_LIB $$DEVIL_LIB $$FREETYPE_LIB $$QUAZIP_LIB $$ZLIB_LIB -lopengl32 -lglu32

UI_DIR = ../sources

SOURCES += \
    sources/MainWindow.cpp \
    sources/QVectorBox.cpp \
    sources/QTBEngine.cpp \
    sources/QNodeBinders.cpp \
    sources/QBrowsEdit.cpp \
    sources/QSignalBlocker.cpp \
    sources/PackerDialog.cpp \
    sources/main.cpp \
    sources/NodeFactory.cpp \
    sources/QLightInteractor.cpp \
    sources/QMapMarkInteractor.cpp \
    sources/QMeshInteractor.cpp \
    sources/QNodeInteractor.cpp \
    sources/QParticlesInteractor.cpp \
    sources/ClassFactory.cpp \
    sources/HistoryState.cpp \
    sources/NodeListProxyModel.cpp \

HEADERS  += \
    sources/MainWindow.h \
    sources/Tools.h \
    sources/QVectorBox.h \
    sources/QTBEngine.h \
    sources/QNodeBinders.h \
    sources/QBrowsEdit.h \
    sources/QSignalBlocker.h \
    sources/PackerDialog.h \
    sources/Metatype.h \
    sources/NodeFactory.h \
    sources/QLightInteractor.h \
    sources/QMapMarkInteractor.h \
    sources/QMeshInteractor.h \
    sources/QNodeInteractor.h \
    sources/QParticlesInteractor.h \
    sources/ClassFactory.h \
    sources/HistoryState.h \
    sources/NodeListProxyModel.h \

FORMS += \
    sources/about.ui \
    sources/interface.ui \
    sources/matedit.ui \
    sources/packer.ui \

RESOURCES += \
    sources/ressource.qrc

RC_FILE += \
    sources/ressource.rc
