/* 
 * File:   QParticlesInteractor.cpp
 * Author: b4n92uid
 * 
 * Created on 29 août 2011, 07:48
 */

#include "QParticlesInteractor.h"
#include "MainWindow.h"

QParticlesInteractor::QParticlesInteractor(MainWindow* mainwin, tbe::scene::ParticlesEmiter* target)
: QNodeInteractor(mainwin, target), m_target(target)
{

}

QParticlesInteractor::~QParticlesInteractor()
{
}

void QParticlesInteractor::setGravity(const tbe::Vector3f& v)
{
    m_target->setGravity(v);

    m_mainwin->notifyChanges(true);
}

void QParticlesInteractor::setBulletsize(const tbe::Vector2f& v)
{
    m_target->setBulletSize(v);

    m_mainwin->notifyChanges(true);
}

void QParticlesInteractor::setBoxsize(const tbe::Vector3f& v)
{
    m_target->setBoxSize(v);

    m_mainwin->notifyChanges(true);
}

void QParticlesInteractor::setFreemove(double v)
{
    m_target->setFreeMove(v);

    m_mainwin->notifyChanges(true);
}

void QParticlesInteractor::setLifeinit(double v)
{
    m_target->setLifeInit(v);

    m_mainwin->notifyChanges(true);
}

void QParticlesInteractor::setLifedown(double v)
{
    m_target->setLifeDown(v);

    m_mainwin->notifyChanges(true);
}

void QParticlesInteractor::setNumber(int v)
{
    m_target->setNumber(v);

    m_mainwin->notifyChanges(true);
}

void QParticlesInteractor::setTexture(const QString& v)
{
    if(m_target && !v.isEmpty())
    {
        try
        {
            m_target->setTexture(v.toStdString());

            m_mainwin->notifyChanges(true);
        }
        catch(std::exception& e)
        {
            QMessageBox::critical(m_mainwin, "Chargement de texture", e.what());
        }
    }

}

void QParticlesInteractor::setPointSprite(bool stat)
{
    m_target->setUsePointSprite(stat);

    m_mainwin->notifyChanges(true);
}

void QParticlesInteractor::setContinousMode(bool stat)
{
    m_target->setContinousMode(stat);

    m_mainwin->notifyChanges(true);
}

void QParticlesInteractor::build()
{
    m_target->build();
}

void QParticlesInteractor::select()
{
    QNodeInteractor::select();

    connect(m_mainwin->nodesGui.particlesTab.gravity, SIGNAL(valueChanged(const tbe::Vector3f&)), this, SLOT(setGravity(const tbe::Vector3f&)));
    connect(m_mainwin->nodesGui.particlesTab.boxsize, SIGNAL(valueChanged(const tbe::Vector3f&)), this, SLOT(setBoxsize(const tbe::Vector3f&)));
    connect(m_mainwin->nodesGui.particlesTab.bulletsize, SIGNAL(valueChanged(const tbe::Vector2f&)), this, SLOT(setBulletsize(const tbe::Vector2f&)));
    connect(m_mainwin->nodesGui.particlesTab.freemove, SIGNAL(valueChanged(double)), this, SLOT(setFreemove(double)));
    connect(m_mainwin->nodesGui.particlesTab.lifeinit, SIGNAL(valueChanged(double)), this, SLOT(setLifeinit(double)));
    connect(m_mainwin->nodesGui.particlesTab.lifedown, SIGNAL(valueChanged(double)), this, SLOT(setLifedown(double)));
    connect(m_mainwin->nodesGui.particlesTab.number, SIGNAL(valueChanged(int)), this, SLOT(setNumber(int)));
    connect(m_mainwin->nodesGui.particlesTab.texture, SIGNAL(textChanged(const QString&)), this, SLOT(setTexture(const QString&)));
    connect(m_mainwin->nodesGui.particlesTab.continiousmode, SIGNAL(clicked(bool)), this, SLOT(setContinousMode(bool)));
    connect(m_mainwin->nodesGui.particlesTab.pointsprite, SIGNAL(clicked(bool)), this, SLOT(setPointSprite(bool)));
    connect(m_mainwin->nodesGui.particlesTab.build, SIGNAL(clicked()), this, SLOT(build()));

    update();


    QStandardItem* item = m_mainwin->nodesGui.nodeItemBinder[this];

    m_mainwin->nodesGui.nodesListView->blockSignals(true);
    m_mainwin->nodesGui.nodesListView->setCurrentIndex(m_mainwin->nodesGui.nodesListModel->indexFromItem(item));
    m_mainwin->nodesGui.nodesListView->blockSignals(false);

    m_mainwin->nodesGui.attribTab->setCurrentIndex(3);
}

void QParticlesInteractor::deselect()
{
    QNodeInteractor::deselect();

    disconnect(m_mainwin->nodesGui.particlesTab.gravity, SIGNAL(valueChanged(const tbe::Vector3f&)), 0, 0);
    disconnect(m_mainwin->nodesGui.particlesTab.boxsize, SIGNAL(valueChanged(const tbe::Vector3f&)), 0, 0);
    disconnect(m_mainwin->nodesGui.particlesTab.bulletsize, SIGNAL(valueChanged(const tbe::Vector2f&)), 0, 0);
    disconnect(m_mainwin->nodesGui.particlesTab.freemove, SIGNAL(valueChanged(double)), 0, 0);
    disconnect(m_mainwin->nodesGui.particlesTab.lifeinit, SIGNAL(valueChanged(double)), 0, 0);
    disconnect(m_mainwin->nodesGui.particlesTab.lifedown, SIGNAL(valueChanged(double)), 0, 0);
    disconnect(m_mainwin->nodesGui.particlesTab.number, SIGNAL(valueChanged(int)), 0, 0);
    disconnect(m_mainwin->nodesGui.particlesTab.texture, SIGNAL(textChanged(const QString&)), 0, 0);
    disconnect(m_mainwin->nodesGui.particlesTab.continiousmode, SIGNAL(clicked(bool)), 0, 0);
    disconnect(m_mainwin->nodesGui.particlesTab.pointsprite, SIGNAL(clicked(bool)), 0, 0);
    disconnect(m_mainwin->nodesGui.particlesTab.build, SIGNAL(clicked()), 0, 0);
}

void QParticlesInteractor::update()
{
    QNodeInteractor::update();

    QSignalBlocker blocker;
    blocker << m_mainwin->nodesGui.particlesTab.gravity << m_mainwin->nodesGui.particlesTab.boxsize
            << m_mainwin->nodesGui.particlesTab.bulletsize << m_mainwin->nodesGui.particlesTab.freemove
            << m_mainwin->nodesGui.particlesTab.lifeinit << m_mainwin->nodesGui.particlesTab.lifedown
            << m_mainwin->nodesGui.particlesTab.number << m_mainwin->nodesGui.particlesTab.texture
            << m_mainwin->nodesGui.particlesTab.continiousmode << m_mainwin->nodesGui.particlesTab.pointsprite;

    blocker.block();

    m_mainwin->nodesGui.particlesTab.gravity->setValue(m_target->getGravity());
    m_mainwin->nodesGui.particlesTab.boxsize->setValue(m_target->getBoxSize());
    m_mainwin->nodesGui.particlesTab.bulletsize->setValue(m_target->getBulletSize());
    m_mainwin->nodesGui.particlesTab.freemove->setValue(m_target->getFreeMove());
    m_mainwin->nodesGui.particlesTab.lifeinit->setValue(m_target->getLifeInit());
    m_mainwin->nodesGui.particlesTab.lifedown->setValue(m_target->getLifeDown());
    m_mainwin->nodesGui.particlesTab.number->setValue(m_target->getNumber());
    m_mainwin->nodesGui.particlesTab.texture->setOpenFileName(QString::fromStdString(m_target->getTexture().getFilename()));
    m_mainwin->nodesGui.particlesTab.continiousmode->setChecked(m_target->isContinousMode());
    m_mainwin->nodesGui.particlesTab.pointsprite->setChecked(m_target->isUsePointSprite());

    blocker.unblock();



    using namespace tbe;
    using namespace scene;

    Grid* grid = m_mainwin->m_tbeWidget->getGrid();
    Box* axe = m_mainwin->m_tbeWidget->getAxe();

    Vector3f size = m_target->getBoxSize();
    size.y = 1;

    axe->setEnable(true);
    axe->setMatrix(m_target->getAbsoluteMatrix());
    axe->setPos(m_target->getAbsoluteMatrix().getPos() + size / 2.0f);
    axe->setSize(size / 2.0f + 0.1f);
    axe->setColor(Vector4f(0, 1, 1, 0.25));

    grid->setPos(Vector3f::Y(axe->getPos().y));
}
