/* 
 * File:   QLight.cpp
 * Author: b4n92uid
 * 
 * Created on 17 mars 2012, 18:43
 */

#include "QLight.h"
#include "MainWindow.h"
#include "QTBEngine.h"

using namespace tbe::scene;

QLight::QLight(MainWindow* mainwin) :
Light(mainwin->tbeWidget()->sceneParser()->getMeshScene()),
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

    m_mainwin->registerNode(this, items);

    Node::setup();
}
