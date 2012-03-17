/* 
 * File:   QMesh.cpp
 * Author: b4n92uid
 * 
 * Created on 17 mars 2012, 18:42
 */

#include "QMesh.h"
#include "MainWindow.h"
#include "QTBEngine.h"

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

    m_mainwin->registerNode(this, items);

    Node::setup();
}