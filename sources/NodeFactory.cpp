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
Mesh(mainwin->getTbeWidget()->getSceneParser()->getMeshScene()),
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

    getParent()->addChild(theclone);

    theclone->setup();
    
    return theclone;
}

void QMesh::setup()
{
    using namespace tbe::scene;

    QVariant userdata;
    userdata.setValue((QNodeInteractor*)this);

    QStandardItem* itemType = new QStandardItem("Mesh");
    itemType->setIcon(QIcon(":/Medias/medias/mesh.png"));
    itemType->setData(userdata);

    QStandardItem* itemName = new QStandardItem(QString::fromStdString(getName()));
    itemName->setData(userdata);

    QItemsList items;
    items << itemType << itemName;

    QNodeInteractor* parent = m_mainwin->m_tbeWidget->fetchInterface(getParent());

    if(m_mainwin->nodesGui.nodeItemBinder.count(parent))
        m_mainwin->nodesGui.nodeItemBinder[parent]->appendRow(items);
    else
        m_mainwin->nodesGui.nodesListModel->appendRow(items);

    m_mainwin->nodesGui.nodeItemBinder[this] = itemType;

    m_mainwin->notifyChanges(true);

    m_mainwin->getTbeWidget()->meshRegister(this);

    Node::setup();
}

QLight::QLight(MainWindow* mainwin) :
Light(mainwin->getTbeWidget()->getSceneParser()->getLightScene()),
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

    getParent()->addChild(theclone);

    theclone->setup();
    
    return theclone;
}

void QLight::setup()
{
    using namespace tbe::scene;

    QVariant userdata;
    userdata.setValue((QNodeInteractor*)this);

    QStandardItem* itemType = new QStandardItem("Light");
    itemType->setIcon(QIcon(":/Medias/medias/light.png"));
    itemType->setData(userdata);

    QStandardItem* itemName = new QStandardItem(QString::fromStdString(getName()));
    itemName->setData(userdata);

    QItemsList items;
    items << itemType << itemName;

    QNodeInteractor* parent = m_mainwin->m_tbeWidget->fetchInterface(getParent());

    if(m_mainwin->nodesGui.nodeItemBinder.count(parent))
        m_mainwin->nodesGui.nodeItemBinder[parent]->appendRow(items);
    else
        m_mainwin->nodesGui.nodesListModel->appendRow(items);

    m_mainwin->nodesGui.nodeItemBinder[this] = itemType;

    m_mainwin->notifyChanges(true);

    m_mainwin->getTbeWidget()->lightRegister(this);

    Node::setup();
}

QParticles::QParticles(MainWindow* mainwin) :
ParticlesEmiter(mainwin->getTbeWidget()->getSceneParser()->getParticlesScene()),
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

    getParent()->addChild(theclone);

    theclone->setup();
    
    return theclone;
}

void QParticles::setup()
{
    using namespace tbe::scene;

    QVariant userdata;
    userdata.setValue((QNodeInteractor*)this);

    QStandardItem* itemType = new QStandardItem("Particles");
    itemType->setIcon(QIcon(":/Medias/medias/particles.png"));
    itemType->setData(userdata);

    QStandardItem* itemName = new QStandardItem(QString::fromStdString(getName()));
    itemName->setData(userdata);

    QItemsList items;
    items << itemType << itemName;

    QNodeInteractor* parent = m_mainwin->m_tbeWidget->fetchInterface(getParent());

    if(m_mainwin->nodesGui.nodeItemBinder.count(parent))
        m_mainwin->nodesGui.nodeItemBinder[parent]->appendRow(items);
    else
        m_mainwin->nodesGui.nodesListModel->appendRow(items);

    m_mainwin->nodesGui.nodeItemBinder[this] = itemType;

    m_mainwin->notifyChanges(true);

    m_mainwin->getTbeWidget()->particlesRegister(this);

    Node::setup();
}

QMapMark::QMapMark(MainWindow* mainwin) :
MapMark(mainwin->getTbeWidget()->getSceneParser()->getMarkScene()),
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

    getParent()->addChild(theclone);

    theclone->setup();
    
    return theclone;
}

void QMapMark::setup()
{
    using namespace tbe::scene;

    QVariant userdata;
    userdata.setValue((QNodeInteractor*)this);

    QStandardItem* itemType = new QStandardItem("Mark");
    itemType->setIcon(QIcon(":/Medias/medias/mark.png"));
    itemType->setData(userdata);

    QStandardItem* itemName = new QStandardItem(QString::fromStdString(getName()));
    itemName->setData(userdata);

    QItemsList items;
    items << itemType << itemName;

    QNodeInteractor* parent = m_mainwin->m_tbeWidget->fetchInterface(getParent());

    if(m_mainwin->nodesGui.nodeItemBinder.count(parent))
        m_mainwin->nodesGui.nodeItemBinder[parent]->appendRow(items);
    else
        m_mainwin->nodesGui.nodesListModel->appendRow(items);

    m_mainwin->nodesGui.nodeItemBinder[this] = itemType;

    m_mainwin->notifyChanges(true);

    m_mainwin->getTbeWidget()->markRegister(this);

    Node::setup();
}
