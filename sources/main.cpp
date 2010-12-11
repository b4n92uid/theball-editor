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

    // initialize resources, if needed
    // Q_INIT_RESOURCE(resfile);

    QApplication app(argc, argv);

    MainWindow window;
    window.show();
    window.openScene("D:\\projets\\cpp\\tbengine\\demo\\bin\\medias\\scene.ballscene");

    return app.exec();
}
