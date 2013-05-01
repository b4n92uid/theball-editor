/* 
 * File:   ClassFactory.cpp
 * Author: b4n92uid
 * 
 * Created on 10 septembre 2011, 12:57
 */

#include "ClassFactory.h"

#include "QNodeInteractor.h"
#include "QMeshInteractor.h"
#include "QLightInteractor.h"
#include "QParticlesInteractor.h"
#include "QMapMarkInteractor.h"

#include "MainWindow.h"

using namespace tbe;
using namespace scene;

ClassFactory::ClassFactory(MainWindow* mainwin)
{
    m_mainwin = mainwin;
}

tbe::scene::Mesh* ClassFactory::newMesh(tbe::scene::MeshParallelScene* scene)
{
    Mesh* mesh = new Mesh(scene);

    return mesh;
}

QMeshInteractor* ClassFactory::setupMeshGui(tbe::scene::Mesh* mesh)
{
    QVariant interface;
    interface.setValue((QNodeInteractor*)this);

    QStandardItem* itemType = new QStandardItem("Mesh");
    itemType->setIcon(QIcon(":/Medias/medias/mesh.png"));
    itemType->setData(interface, ITEM_ROLE_NODE);
    itemType->setData(QString::fromStdString(mesh->getName()), ITEM_ROLE_NAME);
    itemType->setData("Mesh", ITEM_ROLE_TYPE);

    QStandardItem* itemName = new QStandardItem(QString::fromStdString(mesh->getName()));
    itemName->setData(interface, ITEM_ROLE_NODE);
    itemName->setData(QString::fromStdString(mesh->getName()), ITEM_ROLE_NAME);
    itemName->setData("Mesh", ITEM_ROLE_TYPE);

    QItemsList items;
    items << itemType << itemName;

    QMeshInteractor* meshIneractor = new QMeshInteractor(m_mainwin, mesh);

    m_mainwin->registerInteractor(meshIneractor, items);

    return meshIneractor;
}

void ClassFactory::setupMesh(tbe::scene::Mesh* mesh)
{
    setupMeshGui(mesh);
}

tbe::scene::Light* ClassFactory::newLight(tbe::scene::MeshParallelScene* scene)
{
    Light* light = new Light(scene);

    return light;
}

void ClassFactory::setupLight(tbe::scene::Light* light)
{
    setupLightGui(light);
}

QLightInteractor* ClassFactory::setupLightGui(tbe::scene::Light* light)
{
    using namespace tbe::scene;

    QVariant interface;
    interface.setValue((QNodeInteractor*)this);

    QStandardItem* itemType = new QStandardItem("Light");
    itemType->setIcon(QIcon(":/Medias/medias/light.png"));
    itemType->setData(interface, ITEM_ROLE_NODE);
    itemType->setData(QString::fromStdString(light->getName()), ITEM_ROLE_NAME);
    itemType->setData("Light", ITEM_ROLE_TYPE);

    QStandardItem* itemName = new QStandardItem(QString::fromStdString(light->getName()));
    itemName->setData(interface, ITEM_ROLE_NODE);
    itemName->setData(QString::fromStdString(light->getName()), ITEM_ROLE_NAME);
    itemName->setData("Light", ITEM_ROLE_TYPE);

    QItemsList items;
    items << itemType << itemName;

    QLightInteractor* li = new QLightInteractor(m_mainwin, light);
    m_mainwin->registerInteractor(li, items);

    return li;
}

tbe::scene::ParticlesEmiter* ClassFactory::newParticles(tbe::scene::ParticlesParallelScene* scene)
{
    ParticlesEmiter* particles = new ParticlesEmiter(scene);

    return particles;
}

void ClassFactory::setupParticles(tbe::scene::ParticlesEmiter* particles)
{
    setupParticlesGui(particles);
}

QParticlesInteractor* ClassFactory::setupParticlesGui(tbe::scene::ParticlesEmiter* particles)
{
    using namespace tbe::scene;

    QVariant interface;
    interface.setValue((QNodeInteractor*)this);

    QStandardItem* itemType = new QStandardItem("Particles");
    itemType->setIcon(QIcon(":/Medias/medias/particles.png"));
    itemType->setData(interface, ITEM_ROLE_NODE);
    itemType->setData(QString::fromStdString(particles->getName()), ITEM_ROLE_NAME);
    itemType->setData("Particles", ITEM_ROLE_TYPE);

    QStandardItem* itemName = new QStandardItem(QString::fromStdString(particles->getName()));
    itemName->setData(interface, ITEM_ROLE_NODE);
    itemName->setData(QString::fromStdString(particles->getName()), ITEM_ROLE_NAME);
    itemName->setData("Particles", ITEM_ROLE_TYPE);

    QItemsList items;
    items << itemType << itemName;

    QParticlesInteractor* pi = new QParticlesInteractor(m_mainwin, particles);
    m_mainwin->registerInteractor(pi, items);

    return pi;
}

tbe::scene::MapMark* ClassFactory::newMapMark(tbe::scene::MapMarkParallelScene* scene)
{
    MapMark* mapmark = new MapMark(scene);

    return mapmark;
}

void ClassFactory::setupMapMark(tbe::scene::MapMark* mark)
{
    setupMapMarkGui(mark);
}

QMapMarkInteractor* ClassFactory::setupMapMarkGui(tbe::scene::MapMark* mark)
{
    using namespace tbe::scene;

    QVariant interface;
    interface.setValue((QNodeInteractor*)this);

    QStandardItem* itemType = new QStandardItem("Mark");
    itemType->setIcon(QIcon(":/Medias/medias/mark.png"));
    itemType->setData(interface, ITEM_ROLE_NODE);
    itemType->setData(QString::fromStdString(mark->getName()), ITEM_ROLE_NAME);
    itemType->setData("Mark", ITEM_ROLE_TYPE);

    QStandardItem* itemName = new QStandardItem(QString::fromStdString(mark->getName()));
    itemName->setData(interface, ITEM_ROLE_NODE);
    itemName->setData(QString::fromStdString(mark->getName()), ITEM_ROLE_NAME);
    itemName->setData("Mark", ITEM_ROLE_TYPE);

    QItemsList items;
    items << itemType << itemName;

    QMapMarkInteractor* mi = new QMapMarkInteractor(m_mainwin, mark);
    m_mainwin->registerInteractor(mi, items);

    return mi;
}