/* 
 * File:   QMapMarkInteractor.cpp
 * Author: b4n92uid
 * 
 * Created on 29 août 2011, 07:51
 */

#include "QMapMarkInteractor.h"
#include "MainWindow.h"
#include "QTBEngine.h"

QMapMarkInteractor::QMapMarkInteractor(MainWindow* mainwin, tbe::scene::MapMark* target)
: QNodeInteractor(mainwin, target), m_target(target) { }

QMapMarkInteractor::~QMapMarkInteractor() { }

void QMapMarkInteractor::triggerDialog() { }

void QMapMarkInteractor::setup()
{
    QVariant interface;
    interface.setValue((QNodeInteractor*) this);

    QStandardItem* itemType = new QStandardItem("Mark");
    itemType->setIcon(QIcon(":/Medias/medias/mark.png"));
    itemType->setData(interface, ITEM_ROLE_NODE);
    itemType->setData(QString::fromStdString(m_target->getName()), ITEM_ROLE_NAME);
    itemType->setData("Mark", ITEM_ROLE_TYPE);

    QStandardItem* itemName = new QStandardItem(QString::fromStdString(m_target->getName()));
    itemName->setData(interface, ITEM_ROLE_NODE);
    itemName->setData(QString::fromStdString(m_target->getName()), ITEM_ROLE_NAME);
    itemName->setData("Mark", ITEM_ROLE_TYPE);

    QItemsList items;
    items << itemType << itemName;

    m_mainwin->registerInteractor(this, items);

}

QMapMarkInteractor* QMapMarkInteractor::clone()
{
    QMapMarkInteractor* inter = new QMapMarkInteractor(m_mainwin, m_target->clone());
    return inter;
}

QString QMapMarkInteractor::typeName() const
{
    return "MapMark";
}

void QMapMarkInteractor::bindInterface()
{
    QNodeInteractor::bindInterface();

    updateGui();
}

void QMapMarkInteractor::unbindFromGui()
{
    QNodeInteractor::unbindFromGui();
}

void QMapMarkInteractor::updateGui()
{
    QNodeInteractor::updateGui();
}
