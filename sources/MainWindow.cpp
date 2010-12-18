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
    m_envManager = new QEnvManager(this, &m_uinterface);

    // Node --------------------------------------------------------------------

    connect(m_tbeWidget, SIGNAL(notifyMeshAdd(tbe::scene::Mesh*)),
            m_nodsManager, SLOT(meshAdd(tbe::scene::Mesh*)));

    connect(m_tbeWidget, SIGNAL(notifyMeshSelect(tbe::scene::Mesh*)),
            m_nodsManager, SLOT(meshSelect(tbe::scene::Mesh*)));

    connect(m_nodsManager, SIGNAL(notifyMeshAdd(tbe::scene::Mesh*)),
            m_tbeWidget, SLOT(meshAdd(tbe::scene::Mesh*)));

    connect(m_nodsManager, SIGNAL(notifyMeshClone(tbe::scene::Mesh*)),
            m_tbeWidget, SLOT(meshClone(tbe::scene::Mesh*)));

    connect(m_nodsManager, SIGNAL(notifyMeshSelect(tbe::scene::Mesh*)),
            m_tbeWidget, SLOT(meshSelect(tbe::scene::Mesh*)));


    connect(m_tbeWidget, SIGNAL(notifyLightAdd(tbe::scene::Light*)),
            m_nodsManager, SLOT(lightAdd(tbe::scene::Light*)));

    connect(m_tbeWidget, SIGNAL(notifyLightSelect(tbe::scene::Light*)),
            m_nodsManager, SLOT(lightSelect(tbe::scene::Light*)));

    connect(m_nodsManager, SIGNAL(notifyLightNew(tbe::scene::Light*)),
            m_tbeWidget, SLOT(lightAdd(tbe::scene::Light*)));

    connect(m_nodsManager, SIGNAL(notifyLightClone(tbe::scene::Light*)),
            m_tbeWidget, SLOT(lightClone(tbe::scene::Light*)));

    connect(m_nodsManager, SIGNAL(notifyLightSelect(tbe::scene::Light*)),
            m_tbeWidget, SLOT(lightSelect(tbe::scene::Light*)));


    connect(m_nodsManager, SIGNAL(pauseRendring()),
            m_tbeWidget, SLOT(pauseRendring()));

    connect(m_nodsManager, SIGNAL(resumeRendring()),
            m_tbeWidget, SLOT(resumeRendring()));

    // Environment -------------------------------------------------------------

    connect(m_envManager, SIGNAL(sceneAmbiantUpdate(const tbe::Vector3f&)),
            m_tbeWidget, SLOT(sceneAmbiant(const tbe::Vector3f&)));

    connect(m_envManager, SIGNAL(skyboxApply(const QStringList&)),
            m_tbeWidget, SLOT(skyboxApply(const QStringList&)));

    connect(m_envManager, SIGNAL(skyboxClear()),
            m_tbeWidget, SLOT(skyboxClear()));

    connect(m_envManager, SIGNAL(fogApply(tbe::Vector4f, float, float)),
            m_tbeWidget, SLOT(fogApply(tbe::Vector4f, float, float)));

    connect(m_envManager, SIGNAL(fogClear()),
            m_tbeWidget, SLOT(fogClear()));

    connect(m_envManager, SIGNAL(pauseRendring()),
            m_tbeWidget, SLOT(pauseRendring()));

    connect(m_envManager, SIGNAL(resumeRendring()),
            m_tbeWidget, SLOT(resumeRendring()));

    connect(m_tbeWidget, SIGNAL(notifyInitFog(tbe::scene::Fog*)),
            m_envManager, SLOT(fogInit(tbe::scene::Fog*)));

    connect(m_tbeWidget, SIGNAL(notifyInitSkybox(tbe::scene::SkyBox*)),
            m_envManager, SLOT(skyboxInit(tbe::scene::SkyBox*)));

    connect(m_tbeWidget, SIGNAL(notifyInitAmbiant(const tbe::Vector3f&)),
            m_envManager, SLOT(ambiantInit(const tbe::Vector3f&)));


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
