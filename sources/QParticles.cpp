/* 
 * File:   QParticles.cpp
 * Author: b4n92uid
 * 
 * Created on 17 mars 2012, 18:44
 */

#include "QParticles.h"
#include "MainWindow.h"
#include "QTBEngine.h"

using namespace tbe::scene;

QParticles::QParticles(MainWindow* mainwin) :
ParticlesEmiter(mainwin->tbeWidget()->sceneParser()->getParticlesScene()),
QParticlesInteractor(mainwin, this)
{
}

QParticles::QParticles(MainWindow* mainwin, const tbe::scene::ParticlesEmiter& copy) :
ParticlesEmiter(copy), QParticlesInteractor(mainwin, this)
{
}

QParticles::QParticles(const QParticles& copy) :
ParticlesEmiter(copy), QParticlesInteractor(copy.m_mainwin, this)
{
}

QParticles::~QParticles()
{
}

QParticles* QParticles::clone()
{
    QParticles* theclone = new QParticles(*this);

    return theclone;
}

void QParticles::setup()
{
    using namespace tbe::scene;

    QVariant interface;
    interface.setValue((QNodeInteractor*)this);

    QStandardItem* itemType = new QStandardItem("Particles");
    itemType->setIcon(QIcon(":/Medias/medias/particles.png"));
    itemType->setData(interface, ITEM_ROLE_NODE);
    itemType->setData(QString::fromStdString(getName()), ITEM_ROLE_NAME);
    itemType->setData("Particles", ITEM_ROLE_TYPE);

    QStandardItem* itemName = new QStandardItem(QString::fromStdString(getName()));
    itemName->setData(interface, ITEM_ROLE_NODE);
    itemName->setData(QString::fromStdString(getName()), ITEM_ROLE_NAME);
    itemName->setData("Particles", ITEM_ROLE_TYPE);

    QItemsList items;
    items << itemType << itemName;

    m_mainwin->registerNode(this, items);

    Node::setup();
}
