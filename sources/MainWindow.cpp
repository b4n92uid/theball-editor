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
    Ui_mainWindow uinterface;

    uinterface.setupUi(this);

    connect(uinterface.actionOuvrire, SIGNAL(triggered()), this, SLOT(OpenFileDialog()));
    connect(uinterface.actionEnregistrer, SIGNAL(triggered()), this, SLOT(SaveFile()));
    connect(uinterface.actionEnregistrer_sous, SIGNAL(triggered()), this, SLOT(SaveFileDialog()));
    connect(uinterface.actionQuitter, SIGNAL(triggered()), qApp, SLOT(quit()));

    m_tbeWidget = uinterface.glwidget;

    m_infoText = uinterface.infoText;

    m_nodsManager = new QNodesManager(this, &uinterface);
    m_lightsManager = new QLightsManager(this, &uinterface);
    m_envManager = new QEnvManager(this, &uinterface);

    connect(m_tbeWidget, SIGNAL(NotifyMeshAlloc(tbe::scene::Mesh*)),
            m_nodsManager, SLOT(MeshAdd(tbe::scene::Mesh*)));

    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(UpdateGui()));
    timer->start(16);
}

MainWindow::~MainWindow()
{
}

void MainWindow::OpenFileDialog()
{
    QString filename = QFileDialog::getOpenFileName(this);

    if(!filename.isNull())
        OpenFile(filename);
}

void MainWindow::OpenFile(const QString& filename)
{
    m_tbeWidget->loadScene(filename);
}

void MainWindow::SaveFileDialog()
{
    QString filename = QFileDialog::getSaveFileName(this);

    if(!filename.isNull())
        SaveFile(filename);
}

void MainWindow::SaveFile()
{
    SaveFile(m_filename);
}

void MainWindow::SaveFile(const QString& filename)
{
}

void MainWindow::UpdateGui()
{
    m_tbeWidget->fillTextInfo(m_infoText);
}
