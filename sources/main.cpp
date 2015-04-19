/*
 * File:   main.cpp
 * Author: b4n92uid
 *
 * Created on 3 décembre 2010, 17:16
 */

#include <QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    using namespace std;

    #ifdef NDEBUG
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

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    #if QT_VERSION < 0x050000
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    #endif

    QApplication::setOrganizationName("Starfeel Interactive");
    QApplication::setApplicationName("TheBall Editor");

    MainWindow* window = new MainWindow;
    window->initWidgets();
    window->initConnections();
    window->showMaximized();
    window->newScene();

    splash.raise();

    if(argc > 1)
    {
        window->openScene(argv[1]);
    }

    splash.finish(window);

    int ret = app.exec();

    delete window, window = NULL;

    #ifdef NDEBUG
    cout.rdbuf(defaultCout);
    log.close();
    #endif

    return ret;
}
