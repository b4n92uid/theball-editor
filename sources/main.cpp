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
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    QApplication app(argc, argv);

    QApplication::setOrganizationName("b4n92uid software");
    QApplication::setApplicationName("theBall Editor");

    MainWindow window;
    window.initWidgets();
    window.initConnections();
    window.showMaximized();

    #ifdef _DEBUG
    window.openScene("D:\\projets\\cpp\\tbengine\\demos\\bin\\medias\\landscape.scene");
    #endif

    return app.exec();
}
