/*
 * File:   QMeshInteractor.cpp
 * Author: b4n92uid
 *
 * Created on 29 août 2011, 07:23
 */

#include "QMeshInteractor.h"
#include "MainWindow.h"
#include "QTBEngine.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/optional.hpp>

QMeshInteractor::QMeshInteractor(MainWindow* mainwin, tbe::scene::Mesh* target)
: QNodeInteractor(mainwin, target), m_target(target) { }

QMeshInteractor::~QMeshInteractor() { }

void QMeshInteractor::triggerDialog()
{
    m_mainwin->getMeshDialog()->show();
}

void QMeshInteractor::setup()
{
    QVariant interface;
    interface.setValue((QNodeInteractor*) this);

    QStandardItem* itemType = new QStandardItem("Mesh");
    itemType->setIcon(QIcon(":/Medias/medias/mesh.png"));
    itemType->setData(interface, ITEM_ROLE_NODE);
    itemType->setData(QString::fromStdString(m_target->getName()), ITEM_ROLE_NAME);
    itemType->setData("Mesh", ITEM_ROLE_TYPE);

    QStandardItem* itemName = new QStandardItem(QString::fromStdString(m_target->getName()));
    itemName->setData(interface, ITEM_ROLE_NODE);
    itemName->setData(QString::fromStdString(m_target->getName()), ITEM_ROLE_NAME);
    itemName->setData("Mesh", ITEM_ROLE_TYPE);

    QItemsList items;
    items << itemType << itemName;

    m_mainwin->registerInteractor(this, items);
}

QMeshInteractor* QMeshInteractor::clone()
{
    QMeshInteractor* inter = new QMeshInteractor(m_mainwin, m_target->clone());
    return inter;
}

QString QMeshInteractor::typeName() const
{
    return "Mesh";
}

void QMeshInteractor::setBillBoardX(bool x)
{
    tbe::Vector2b apply = m_target->getBillBoard();
    apply.x = x;

    m_target->setBillBoard(apply);
}

void QMeshInteractor::setBillBoardY(bool y)
{
    tbe::Vector2b apply = m_target->getBillBoard();
    apply.y = y;

    m_target->setBillBoard(apply);
}

void QMeshInteractor::setCastShadow(bool s)
{
    m_target->setCastShadow(s);
}

void QMeshInteractor::setReceiveShadow(bool s)
{
    m_target->setReceiveShadow(s);
}

void QMeshInteractor::setComputeNormal()
{
    m_target->computeNormal();
}

void QMeshInteractor::setComputeTangent()
{
    m_target->computeTangent();
}

void QMeshInteractor::bindInterface()
{
    QNodeInteractor::bindInterface();

    m_mainwin->getMeshDialog()->bind(this);

    updateGui();
}

void QMeshInteractor::unbindFromGui()
{
    QNodeInteractor::unbindFromGui();

    m_mainwin->getMeshDialog()->unbind();
}

void QMeshInteractor::updateGui()
{
    using namespace std;
    using namespace tbe::scene;
    using namespace boost;

    QNodeInteractor::updateGui();
    
    m_mainwin->getMeshDialog()->update(m_target);

    m_mainwin->tbeWidget()->highlight(this);
}

void QMeshInteractor::attachMaterial(QString filename)
{
    m_target->attachMaterialFile(filename.toStdString());
    updateGui();
}

void QMeshInteractor::reloadMaterial()
{
    try
    {
        m_target->attachMaterialFile(m_target->getMaterialFile());
        updateGui();
    }
    catch(std::exception& e)
    {
        QMessageBox::critical(m_mainwin, "Erreur rafrichisement", e.what());
    }
}

void QMeshInteractor::releaseMaterial()
{
    m_target->releaseMaterialFile();
    updateGui();
}
