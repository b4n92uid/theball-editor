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
    using namespace std;

    #ifndef _DEBUG
    ofstream log("tbeditor.log");
    streambuf* defaultCout = cout.rdbuf();
    cout.rdbuf(log.rdbuf());
    #endif

    QApplication app(argc, argv);

    QSplashScreen splash(QPixmap(":/Medias/medias/splashscreen.png"));
    splash.show();

    QTranslator translator;
    translator.load("qt_fr", QLibraryInfo::location(QLibraryInfo::TranslationsPath));

    app.installTranslator(&translator);

    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    QApplication::setStyle(new QCleanlooksStyle);
    QApplication::setOrganizationName("b4n92uid software");
    QApplication::setApplicationName("theBall Editor");

    MainWindow window;
    window.initWidgets();
    window.initConnections();
    window.showMaximized();
    window.newScene();

    splash.raise();

    if(argc > 1)
    {
        window.openScene(argv[1]);
    }

    splash.finish(&window);

    int ret = app.exec();

    #ifndef _DEBUG
    cout.rdbuf(defaultCout);
    log.close();
    #endif

    return ret;
}
