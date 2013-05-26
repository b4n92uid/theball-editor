/* 
 * File:   QParticlesInteractor.cpp
 * Author: b4n92uid
 * 
 * Created on 29 août 2011, 07:48
 */

#include "QParticlesInteractor.h"
#include "MainWindow.h"
#include "QTBEngine.h"

QParticlesInteractor::QParticlesInteractor(MainWindow* mainwin, tbe::scene::ParticlesEmiter* target)
: QNodeInteractor(mainwin, target), m_target(target) { }

QParticlesInteractor::~QParticlesInteractor() { }

void QParticlesInteractor::triggerDialog()
{
    m_mainwin->getParticlesDialog()->show();
}

void QParticlesInteractor::setup()
{
    QVariant interface;
    interface.setValue((QNodeInteractor*) this);

    QStandardItem* itemType = new QStandardItem("Particles");
    itemType->setIcon(QIcon(":/Medias/medias/particles.png"));
    itemType->setData(interface, ITEM_ROLE_NODE);
    itemType->setData(QString::fromStdString(m_target->getName()), ITEM_ROLE_NAME);
    itemType->setData("Particles", ITEM_ROLE_TYPE);

    QStandardItem* itemName = new QStandardItem(QString::fromStdString(m_target->getName()));
    itemName->setData(interface, ITEM_ROLE_NODE);
    itemName->setData(QString::fromStdString(m_target->getName()), ITEM_ROLE_NAME);
    itemName->setData("Particles", ITEM_ROLE_TYPE);

    QItemsList items;
    items << itemType << itemName;

    m_mainwin->registerInteractor(this, items);

}

QParticlesInteractor* QParticlesInteractor::clone()
{
    QParticlesInteractor* inter = new QParticlesInteractor(m_mainwin, m_target->clone());
    return inter;
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

void QParticlesInteractor::bindWithGui()
{
    QNodeInteractor::bindWithGui();

    m_mainwin->getParticlesDialog()->bind(this);

    updateGui();
}

void QParticlesInteractor::unbindFromGui()
{
    QNodeInteractor::unbindFromGui();

    m_mainwin->getParticlesDialog()->unbind();
}

void QParticlesInteractor::updateGui()
{
    QNodeInteractor::updateGui();

    m_mainwin->getParticlesDialog()->update(m_target);

    m_mainwin->tbeWidget()->highlight(this);
}
