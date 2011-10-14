/* 
 * File:   NodeFactory.cpp
 * Author: b4n92uid
 * 
 * Created on 29 août 2011, 08:07
 */

#include "NodeFactory.h"

#include "QMeshInteractor.h"
#include "QParticlesInteractor.h"
#include "QParticlesInteractor.h"
#include "QLightInteractor.h"
#include "QMapMarkInteractor.h"

#include "MainWindow.h"

using namespace tbe::scene;

QMesh::QMesh(MainWindow* mainwin) :
Mesh(mainwin->tbeWidget()->sceneParser()->getMeshScene()),
QMeshInteractor(mainwin, this)
{
}

QMesh::QMesh(MainWindow* mainwin, const tbe::scene::Mesh& copy) :
Mesh(copy), QMeshInteractor(mainwin, this)
{
}

QMesh::QMesh(const QMesh& copy) :
Mesh(copy), QMeshInteractor(copy.m_mainwin, this)
{
}

QMesh::~QMesh()
{
}

QMesh* QMesh::clone()
{
    QMesh* theclone = new QMesh(*this);

    return theclone;
}

void QMesh::setup()
{
    using namespace tbe::scene;

    QVariant interface;
    interface.setValue((QNodeInteractor*)this);

    QStandardItem* itemType = new QStandardItem("Mesh");
    itemType->setIcon(QIcon(":/Medias/medias/mesh.png"));
    itemType->setData(interface, ITEM_ROLE_NODE);
    itemType->setData(QString::fromStdString(getName()), ITEM_ROLE_NAME);
    itemType->setData("Mesh", ITEM_ROLE_TYPE);

    QStandardItem* itemName = new QStandardItem(QString::fromStdString(getName()));
    itemName->setData(interface, ITEM_ROLE_NODE);
    itemName->setData(QString::fromStdString(getName()), ITEM_ROLE_NAME);
    itemName->setData("Mesh", ITEM_ROLE_TYPE);

    QItemsList items;
    items << itemType << itemName;

    m_mainwin->reg(this, items);

    Node::setup();
}

QLight::QLight(MainWindow* mainwin) :
Light(mainwin->tbeWidget()->sceneParser()->getLightScene()),
QLightInteractor(mainwin, this)
{
}

QLight::QLight(MainWindow* mainwin, const tbe::scene::Light& copy) :
Light(copy), QLightInteractor(mainwin, this)
{

}

QLight::QLight(const QLight& copy) :
Light(copy), QLightInteractor(copy.m_mainwin, this)
{
}

QLight::~QLight()
{
}

QLight* QLight::clone()
{
    QLight* theclone = new QLight(*this);

    return theclone;
}

void QLight::setup()
{
    using namespace tbe::scene;

    QVariant interface;
    interface.setValue((QNodeInteractor*)this);

    QStandardItem* itemType = new QStandardItem("Light");
    itemType->setIcon(QIcon(":/Medias/medias/light.png"));
    itemType->setData(interface, ITEM_ROLE_NODE);
    itemType->setData(QString::fromStdString(getName()), ITEM_ROLE_NAME);
    itemType->setData("Light", ITEM_ROLE_TYPE);

    QStandardItem* itemName = new QStandardItem(QString::fromStdString(getName()));
    itemName->setData(interface, ITEM_ROLE_NODE);
    itemName->setData(QString::fromStdString(getName()), ITEM_ROLE_NAME);
    itemName->setData("Light", ITEM_ROLE_TYPE);

    QItemsList items;
    items << itemType << itemName;

    m_mainwin->reg(this, items);

    Node::setup();
}

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

    m_mainwin->reg(this, items);

    Node::setup();
}

QMapMark::QMapMark(MainWindow* mainwin) :
MapMark(mainwin->tbeWidget()->sceneParser()->getMarkScene()),
QMapMarkInteractor(mainwin, this)
{
}

QMapMark::QMapMark(MainWindow* mainwin, const tbe::scene::MapMark& copy) :
MapMark(copy), QMapMarkInteractor(mainwin, this)
{

}

QMapMark::QMapMark(const QMapMark& copy) :
MapMark(copy), QMapMarkInteractor(copy.m_mainwin, this)
{
}

QMapMark::~QMapMark()
{
}

QMapMark* QMapMark::clone()
{
    QMapMark* theclone = new QMapMark(*this);

    return theclone;
}

void QMapMark::setup()
{
    using namespace tbe::scene;

    QVariant interface;
    interface.setValue((QNodeInteractor*)this);

    QStandardItem* itemType = new QStandardItem("Mark");
    itemType->setIcon(QIcon(":/Medias/medias/mark.png"));
    itemType->setData(interface, ITEM_ROLE_NODE);
    itemType->setData(QString::fromStdString(getName()), ITEM_ROLE_NAME);
    itemType->setData("Mark", ITEM_ROLE_TYPE);

    QStandardItem* itemName = new QStandardItem(QString::fromStdString(getName()));
    itemName->setData(interface, ITEM_ROLE_NODE);
    itemName->setData(QString::fromStdString(getName()), ITEM_ROLE_NAME);
    itemName->setData("Mark", ITEM_ROLE_TYPE);

    QItemsList items;
    items << itemType << itemName;

    m_mainwin->reg(this, items);

    Node::setup();
}
