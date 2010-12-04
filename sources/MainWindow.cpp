/* 
 * File:   MainWindow.cpp
 * Author: b4n92uid
 * 
 * Created on 3 décembre 2010, 17:20
 */

#include "MainWindow.h"
#include "ui_interface.h"

MainWindow::MainWindow()
{
    Ui_mainWindow* uinterface = new Ui_mainWindow;

    uinterface->setupUi(this);

    m_nodsManager = new QNodesManager(uinterface);
    m_lightsManager = new QLightsManager(uinterface);
    m_envManager = new QEnvManager(uinterface);
}

MainWindow::~MainWindow()
{
}
