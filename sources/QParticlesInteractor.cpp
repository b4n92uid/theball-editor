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

QString QParticlesInteractor::typeName() const
{
    return "Particles";
}

void QParticlesInteractor::setGravity(const tbe::Vector3f& v)
{
    m_target->setGravity(v);
}

void QParticlesInteractor::setBulletsize(const tbe::Vector2f& v)
{
    m_target->setBulletSize(v);
}

void QParticlesInteractor::setBoxsize(const tbe::Vector3f& v)
{
    m_target->setBoxSize(v);
}

void QParticlesInteractor::setFreemove(double v)
{
    m_target->setFreeMove(v);
}

void QParticlesInteractor::setLifeinit(double v)
{
    m_target->setLifeInit(v);
}

void QParticlesInteractor::setLifedown(double v)
{
    m_target->setLifeDown(v);
}

void QParticlesInteractor::setNumber(int v)
{
    m_target->setNumber(v);
}

void QParticlesInteractor::setTexture(const QString& v)
{
    if(m_target && !v.isEmpty())
    {
        try
        {
            m_target->setTexture(v.toStdString());
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
}

void QParticlesInteractor::setContinousMode(bool stat)
{
    m_target->setContinousMode(stat);
}

void QParticlesInteractor::build()
{
    m_target->build();
}

void QParticlesInteractor::select()
{
    QNodeInteractor::select();

    connect(m_mainwin->nodesGui.particles.gravity, SIGNAL(valueChanged(const tbe::Vector3f&)), this, SLOT(setGravity(const tbe::Vector3f&)));
    connect(m_mainwin->nodesGui.particles.boxsize, SIGNAL(valueChanged(const tbe::Vector3f&)), this, SLOT(setBoxsize(const tbe::Vector3f&)));
    connect(m_mainwin->nodesGui.particles.bulletsize, SIGNAL(valueChanged(const tbe::Vector2f&)), this, SLOT(setBulletsize(const tbe::Vector2f&)));
    connect(m_mainwin->nodesGui.particles.freemove, SIGNAL(valueChanged(double)), this, SLOT(setFreemove(double)));
    connect(m_mainwin->nodesGui.particles.lifeinit, SIGNAL(valueChanged(double)), this, SLOT(setLifeinit(double)));
    connect(m_mainwin->nodesGui.particles.lifedown, SIGNAL(valueChanged(double)), this, SLOT(setLifedown(double)));
    connect(m_mainwin->nodesGui.particles.number, SIGNAL(valueChanged(int)), this, SLOT(setNumber(int)));
    connect(m_mainwin->nodesGui.particles.texture, SIGNAL(textChanged(const QString&)), this, SLOT(setTexture(const QString&)));
    connect(m_mainwin->nodesGui.particles.continiousmode, SIGNAL(clicked(bool)), this, SLOT(setContinousMode(bool)));
    connect(m_mainwin->nodesGui.particles.pointsprite, SIGNAL(clicked(bool)), this, SLOT(setPointSprite(bool)));
    connect(m_mainwin->nodesGui.particles.build, SIGNAL(clicked()), this, SLOT(build()));

    update();

    m_mainwin->nodesGui.attribTab->setCurrentIndex(3);
}

void QParticlesInteractor::deselect()
{
    QNodeInteractor::deselect();

    disconnect(m_mainwin->nodesGui.particles.gravity, SIGNAL(valueChanged(const tbe::Vector3f&)), 0, 0);
    disconnect(m_mainwin->nodesGui.particles.boxsize, SIGNAL(valueChanged(const tbe::Vector3f&)), 0, 0);
    disconnect(m_mainwin->nodesGui.particles.bulletsize, SIGNAL(valueChanged(const tbe::Vector2f&)), 0, 0);
    disconnect(m_mainwin->nodesGui.particles.freemove, SIGNAL(valueChanged(double)), 0, 0);
    disconnect(m_mainwin->nodesGui.particles.lifeinit, SIGNAL(valueChanged(double)), 0, 0);
    disconnect(m_mainwin->nodesGui.particles.lifedown, SIGNAL(valueChanged(double)), 0, 0);
    disconnect(m_mainwin->nodesGui.particles.number, SIGNAL(valueChanged(int)), 0, 0);
    disconnect(m_mainwin->nodesGui.particles.texture, SIGNAL(textChanged(const QString&)), 0, 0);
    disconnect(m_mainwin->nodesGui.particles.continiousmode, SIGNAL(clicked(bool)), 0, 0);
    disconnect(m_mainwin->nodesGui.particles.pointsprite, SIGNAL(clicked(bool)), 0, 0);
    disconnect(m_mainwin->nodesGui.particles.build, SIGNAL(clicked()), 0, 0);
}

void QParticlesInteractor::update()
{
    QNodeInteractor::update();

    QSignalBlocker blocker;
    blocker << m_mainwin->nodesGui.particles.gravity << m_mainwin->nodesGui.particles.boxsize
            << m_mainwin->nodesGui.particles.bulletsize << m_mainwin->nodesGui.particles.freemove
            << m_mainwin->nodesGui.particles.lifeinit << m_mainwin->nodesGui.particles.lifedown
            << m_mainwin->nodesGui.particles.number << m_mainwin->nodesGui.particles.texture
            << m_mainwin->nodesGui.particles.continiousmode << m_mainwin->nodesGui.particles.pointsprite;

    blocker.block();

    m_mainwin->nodesGui.particles.gravity->setValue(m_target->getGravity());
    m_mainwin->nodesGui.particles.boxsize->setValue(m_target->getBoxSize());
    m_mainwin->nodesGui.particles.bulletsize->setValue(m_target->getBulletSize());
    m_mainwin->nodesGui.particles.freemove->setValue(m_target->getFreeMove());
    m_mainwin->nodesGui.particles.lifeinit->setValue(m_target->getLifeInit());
    m_mainwin->nodesGui.particles.lifedown->setValue(m_target->getLifeDown());
    m_mainwin->nodesGui.particles.number->setValue(m_target->getNumber());
    m_mainwin->nodesGui.particles.texture->setOpenFileName(QString::fromStdString(m_target->getTexture().getFilename()));
    m_mainwin->nodesGui.particles.continiousmode->setChecked(m_target->isContinousMode());
    m_mainwin->nodesGui.particles.pointsprite->setChecked(m_target->isUsePointSprite());

    blocker.unblock();

    m_mainwin->m_tbeWidget->selBox()->setAround(m_target, tbe::Vector4f(0, 1, 1, 0.25));
}
