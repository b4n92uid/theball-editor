/* 
 * File:   QLightInteractor.cpp
 * Author: b4n92uid
 * 
 * Created on 29 août 2011, 07:46
 */

#include "QLightInteractor.h"
#include "MainWindow.h"

QLightInteractor::QLightInteractor(MainWindow* mainwin, tbe::scene::Light* target)
: QNodeInteractor(mainwin, target), m_target(target)
{

}

QLightInteractor::~QLightInteractor()
{
}

void QLightInteractor::setup()
{
    using namespace tbe::scene;

    QVariant userdata;
    userdata.setValue((QNodeInteractor*)this);

    QStandardItem* itemType = new QStandardItem("Light");
    itemType->setIcon(QIcon(":/Medias/medias/light.png"));
    itemType->setData(userdata);

    QStandardItem* itemName = new QStandardItem(QString::fromStdString(m_target->getName()));
    itemName->setData(userdata);

    QItemsList items;
    items << itemType << itemName;

    QNodeInteractor* parent = m_mainwin->m_tbeWidget->fetchInterface(m_target->getParent());

    if(m_mainwin->nodesGui.nodeItemBinder.count(parent))
        m_mainwin->nodesGui.nodeItemBinder[parent]->appendRow(items);
    else
        m_mainwin->nodesGui.nodesListModel->appendRow(items);

    m_mainwin->nodesGui.nodeItemBinder[this] = itemType;

    m_mainwin->notifyChanges(true);
}

void QLightInteractor::setType(int type)
{
    m_target->setType((tbe::scene::Light::Type)type);

    m_mainwin->notifyChanges(true);
}

void QLightInteractor::setAmbiant(const tbe::Vector3f& value)
{
    m_target->setAmbient(tbe::math::vec34(value));

    m_mainwin->notifyChanges(true);
}

void QLightInteractor::setDiffuse(const tbe::Vector3f& value)
{
    m_target->setDiffuse(tbe::math::vec34(value));

    m_mainwin->notifyChanges(true);
}

void QLightInteractor::setSpecular(const tbe::Vector3f& value)
{
    m_target->setSpecular(tbe::math::vec34(value));

    m_mainwin->notifyChanges(true);
}

void QLightInteractor::setRadius(double value)
{
    m_target->setRadius((float)value);

    m_mainwin->notifyChanges(true);
}

void QLightInteractor::select()
{
    deselect();

    QNodeInteractor::select();

    connect(m_mainwin->nodesGui.lighTab.type, SIGNAL(activated(int)), this, SLOT(setType(int)));
    connect(m_mainwin->nodesGui.lighTab.ambiant, SIGNAL(valueChanged(const tbe::Vector3f&)), this, SLOT(setAmbiant(const tbe::Vector3f&)));
    connect(m_mainwin->nodesGui.lighTab.diffuse, SIGNAL(valueChanged(const tbe::Vector3f&)), this, SLOT(setDiffuse(const tbe::Vector3f&)));
    connect(m_mainwin->nodesGui.lighTab.specular, SIGNAL(valueChanged(const tbe::Vector3f&)), this, SLOT(setSpecular(const tbe::Vector3f&)));
    connect(m_mainwin->nodesGui.lighTab.radius, SIGNAL(valueChanged(double)), this, SLOT(setRadius(double)));

    update();


    QStandardItem* item = m_mainwin->nodesGui.nodeItemBinder[this];

    m_mainwin->nodesGui.nodesListView->blockSignals(true);
    m_mainwin->nodesGui.nodesListView->setCurrentIndex(m_mainwin->nodesGui.nodesListModel->indexFromItem(item));
    m_mainwin->nodesGui.nodesListView->blockSignals(false);

    m_mainwin->nodesGui.attribTab->setCurrentIndex(1);
}

void QLightInteractor::deselect()
{
    QNodeInteractor::deselect();

    disconnect(m_mainwin->nodesGui.lighTab.type, SIGNAL(activated(int)), 0, 0);
    disconnect(m_mainwin->nodesGui.lighTab.ambiant, SIGNAL(valueChanged(const tbe::Vector3f&)), 0, 0);
    disconnect(m_mainwin->nodesGui.lighTab.diffuse, SIGNAL(valueChanged(const tbe::Vector3f&)), 0, 0);
    disconnect(m_mainwin->nodesGui.lighTab.specular, SIGNAL(valueChanged(const tbe::Vector3f&)), 0, 0);
    disconnect(m_mainwin->nodesGui.lighTab.radius, SIGNAL(valueChanged(double)), 0, 0);
}

void QLightInteractor::update()
{
    QNodeInteractor::update();

    QSignalBlocker blocker;
    blocker << m_mainwin->nodesGui.lighTab.type << m_mainwin->nodesGui.lighTab.ambiant
            << m_mainwin->nodesGui.lighTab.diffuse << m_mainwin->nodesGui.lighTab.specular
            << m_mainwin->nodesGui.lighTab.radius;

    blocker.block();

    m_mainwin->nodesGui.lighTab.type->setCurrentIndex((int)m_target->getType());
    m_mainwin->nodesGui.lighTab.ambiant->setValue(tbe::math::vec43(m_target->getAmbient()));
    m_mainwin->nodesGui.lighTab.diffuse->setValue(tbe::math::vec43(m_target->getDiffuse()));
    m_mainwin->nodesGui.lighTab.specular->setValue(tbe::math::vec43(m_target->getSpecular()));
    m_mainwin->nodesGui.lighTab.radius->setValue(m_target->getRadius());

    blocker.unblock();


    using namespace tbe;
    using namespace scene;

    Grid* grid = m_mainwin->m_tbeWidget->getGrid();
    Box* axe = m_mainwin->m_tbeWidget->getAxe();

    AABB selAabb = m_target->getAabb();

    axe->setEnable(true);
    axe->setMatrix(m_target->getAbsoluteMatrix());
    axe->setPos(m_target->getAbsoluteMatrix() * selAabb.getCenter());
    axe->setSize(0.25);
    axe->setColor(Vector4f(1, 1, 1, 0.25));

    grid->setPos(Vector3f::Y(axe->getPos().y));
}
