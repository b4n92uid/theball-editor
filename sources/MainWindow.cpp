/* 
 * File:   MainWindow.cpp
 * Author: b4n92uid
 * 
 * Created on 3 décembre 2010, 17:20
 */

#include "MainWindow.h"

MainWindow::MainWindow()
{
    m_uinterface.setupUi(this);

    connect(m_uinterface.actionOuvrire, SIGNAL(triggered()), this, SLOT(openSceneDialog()));
    connect(m_uinterface.actionEnregistrer, SIGNAL(triggered()), this, SLOT(saveScene()));
    connect(m_uinterface.actionEnregistrer_sous, SIGNAL(triggered()), this, SLOT(saveSceneDialog()));
    connect(m_uinterface.actionQuitter, SIGNAL(triggered()), qApp, SLOT(quit()));

    m_tbeWidget = m_uinterface.glwidget;

    m_infoText = m_uinterface.infoText;

    m_nodsManager = new QNodesManager(this, &m_uinterface);
    m_lightsManager = new QLightsManager(this, &m_uinterface);
    m_envManager = new QEnvManager(this, &m_uinterface);

    connect(m_tbeWidget, SIGNAL(notifyMeshAdd(tbe::scene::Mesh*)),
            m_nodsManager, SLOT(meshAdd(tbe::scene::Mesh*)));

    connect(m_tbeWidget, SIGNAL(notifyMeshSelect(tbe::scene::Mesh*)),
            m_nodsManager, SLOT(meshSelect(tbe::scene::Mesh*)));

    connect(m_nodsManager, SIGNAL(notifyMeshAdd(tbe::scene::Mesh*)),
            m_tbeWidget, SLOT(meshAdd(tbe::scene::Mesh*)));

    connect(m_nodsManager, SIGNAL(notifyMeshSelect(tbe::scene::Mesh*)),
            m_tbeWidget, SLOT(meshSelect(tbe::scene::Mesh*)));

    connect(m_nodsManager, SIGNAL(pauseRendring()), m_tbeWidget, SLOT(pauseRendring()));
    connect(m_nodsManager, SIGNAL(resumeRendring()), m_tbeWidget, SLOT(resumeRendring()));

    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(updateGui()));
    m_timer->start(16);
}

MainWindow::~MainWindow()
{
}

void MainWindow::openSceneDialog()
{
    m_tbeWidget->pauseRendring();

    QString filename = QFileDialog::getOpenFileName(this);

    if(!filename.isNull())
        openScene(filename);

    m_tbeWidget->resumeRendring();
}

void MainWindow::openScene(const QString& filename)
{
    QFileInfo filei(filename);
    
    QDir::setCurrent(filei.path());

    m_tbeWidget->loadScene(filename);
}

void MainWindow::saveSceneDialog()
{
    QString filename = QFileDialog::getSaveFileName(this);

    if(!filename.isNull())
        saveScene(filename);
}

void MainWindow::saveScene()
{
    saveScene(m_filename);
}

void MainWindow::saveScene(const QString& filename)
{
    m_tbeWidget->saveScene(filename);
}

void MainWindow::updateGui()
{
    m_tbeWidget->fillTextInfo(m_infoText);
}
