/*
 * File:   main.cpp
 * Author: b4n92uid
 *
 * Created on 3 décembre 2010, 17:16
 */

#include <QtGui/QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QTranslator translator;
    translator.load("qt_fr", QLibraryInfo::location(QLibraryInfo::TranslationsPath));

    app.installTranslator(&translator);

    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    QApplication::setOrganizationName("b4n92uid software");
    QApplication::setApplicationName("theBall Editor");

    MainWindow window;
    window.initWidgets();
    window.initConnections();
    window.showMaximized();
    window.newScene();

    #ifdef _DEBUG
    window.openScene("D:\\projets\\cpp\\theball\\dist\\Release\\MinGW-Windows\\data\\prototype");
    #endif

    return app.exec();
}
