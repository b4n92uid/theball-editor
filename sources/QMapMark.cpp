/* 
 * File:   QMapMark.cpp
 * Author: b4n92uid
 * 
 * Created on 17 mars 2012, 18:46
 */

#include "QMapMark.h"
#include "MainWindow.h"
#include "QTBEngine.h"

using namespace tbe::scene;

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

    m_mainwin->registerNode(this, items);

    Node::setup();
}
