/*
 * File:   QMeshInteractor.cpp
 * Author: b4n92uid
 *
 * Created on 29 aoÃƒÆ’Ã‚Â»t 2011, 07:23
 */

#include "QMeshInteractor.h"
#include "MainWindow.h"
#include "QTBEngine.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/optional.hpp>

QMeshInteractor::QMeshInteractor(MainWindow* mainwin, tbe::scene::Mesh* target)
: QNodeInteractor(mainwin, target), m_target(target)
{
    m_materialDialog = NULL;
}

QMeshInteractor::~QMeshInteractor() { }

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

void QMeshInteractor::setBillBoard()
{
    tbe::Vector2b apply;
    apply.x = m_mainwin->nodesGui.mesh.billboardX->isChecked();
    apply.y = m_mainwin->nodesGui.mesh.billboardY->isChecked();

    m_target->setBillBoard(apply);
}

void QMeshInteractor::setShadow()
{
    m_target->setCastShadow(m_mainwin->nodesGui.mesh.castshadow->isChecked());
    m_target->setReceiveShadow(m_mainwin->nodesGui.mesh.receiveshadow->isChecked());
}

void QMeshInteractor::reloadMaterial()
{
    try
    {
        m_target->attachMaterialFile(m_target->getMaterialFile());
    }
    catch(std::exception& e)
    {
        QMessageBox::critical(m_mainwin, "Erreur rafrichisement", e.what());
    }
}

void QMeshInteractor::openMaterialDialog()
{
    using namespace std;

    if(!m_materialDialog)
    {
        QString filename = QFileDialog::getSaveFileName(m_mainwin, "", m_mainwin->openFileName());

        if(!filename.isEmpty())
        {
            QFile(filename).open(QFile::WriteOnly);

            m_target->attachMaterialFile(filename.toStdString());

            m_materialDialog = new MaterialDialog(m_mainwin, m_target, filename);
            m_materialDialog->bind();
            updateGui();
            m_materialDialog->show();
        }
        else
            return;
    }
    else
    {
        m_materialDialog->bind();
        m_materialDialog->show();
    }
}

void QMeshInteractor::attachMaterial()
{
    QString filename = QFileDialog::getOpenFileName(m_mainwin, "", m_mainwin->openFileName());

    if(!filename.isEmpty())
    {
        m_target->attachMaterialFile(filename.toStdString());

        m_mainwin->nodesGui.mesh.reloadMaterial->setEnabled(true);
        m_mainwin->nodesGui.mesh.releaseMaterial->setEnabled(true);

        m_materialDialog = new MaterialDialog(m_mainwin, m_target, filename);
        m_materialDialog->bind();
        updateGui();
    }
}

void QMeshInteractor::releaseMaterial()
{
    m_target->releaseMaterialFile();
    m_mainwin->nodesGui.mesh.reloadMaterial->setEnabled(false);
    m_mainwin->nodesGui.mesh.releaseMaterial->setEnabled(false);

    m_mainwin->nodesGui.mesh.matinfo->setText("<span style=\" font-style:italic; color:#6a6a6a;\">"
                                              "[Aucun MatÃ©riau chargÃ© pour ce mailliage]</span>");

    m_materialDialog->deleteLater();
    m_materialDialog = NULL;
}

void QMeshInteractor::bindWithGui()
{
    QNodeInteractor::bindWithGui();

    connect(m_mainwin->nodesGui.mesh.editMaterial, SIGNAL(clicked()), this, SLOT(openMaterialDialog()));
    connect(m_mainwin->nodesGui.mesh.attachMaterial, SIGNAL(clicked()), this, SLOT(attachMaterial()));
    connect(m_mainwin->nodesGui.mesh.releaseMaterial, SIGNAL(clicked()), this, SLOT(releaseMaterial()));
    connect(m_mainwin->nodesGui.mesh.reloadMaterial, SIGNAL(clicked()), this, SLOT(reloadMaterial()));

    connect(m_mainwin->nodesGui.mesh.billboardX, SIGNAL(clicked()), this, SLOT(setBillBoard()));
    connect(m_mainwin->nodesGui.mesh.billboardY, SIGNAL(clicked()), this, SLOT(setBillBoard()));

    connect(m_mainwin->nodesGui.mesh.castshadow, SIGNAL(clicked()), this, SLOT(setShadow()));
    connect(m_mainwin->nodesGui.mesh.receiveshadow, SIGNAL(clicked()), this, SLOT(setShadow()));

    if(m_materialDialog)
        m_materialDialog->bind();

    updateGui();

    m_mainwin->nodesGui.attribTab->setCurrentIndex(0);
}

void QMeshInteractor::unbindFromGui()
{
    QNodeInteractor::unbindFromGui();

    disconnect(m_mainwin->nodesGui.mesh.editMaterial, SIGNAL(clicked()), 0, 0);
    disconnect(m_mainwin->nodesGui.mesh.attachMaterial, SIGNAL(clicked()), 0, 0);
    disconnect(m_mainwin->nodesGui.mesh.releaseMaterial, SIGNAL(clicked()), 0, 0);
    disconnect(m_mainwin->nodesGui.mesh.reloadMaterial, SIGNAL(clicked()), 0, 0);

    disconnect(m_mainwin->nodesGui.mesh.billboardX, SIGNAL(clicked()), 0, 0);
    disconnect(m_mainwin->nodesGui.mesh.billboardY, SIGNAL(clicked()), 0, 0);

    disconnect(m_mainwin->nodesGui.mesh.castshadow, SIGNAL(clicked()), 0, 0);
    disconnect(m_mainwin->nodesGui.mesh.receiveshadow, SIGNAL(clicked()), 0, 0);

    if(m_materialDialog)
        m_materialDialog->unbind();

    m_mainwin->nodesGui.mesh.billboardX->setChecked(false);
    m_mainwin->nodesGui.mesh.billboardY->setChecked(false);
    m_mainwin->nodesGui.mesh.castshadow->setChecked(false);
    m_mainwin->nodesGui.mesh.receiveshadow->setChecked(false);
}

void QMeshInteractor::updateGui()
{
    using namespace std;
    using namespace tbe::scene;
    using namespace boost;

    QNodeInteractor::updateGui();

    QSignalBlocker blocker;
    blocker
            << m_mainwin->nodesGui.mesh.billboardX
            << m_mainwin->nodesGui.mesh.billboardY
            << m_mainwin->nodesGui.mesh.castshadow
            << m_mainwin->nodesGui.mesh.receiveshadow
            << m_mainwin->nodesGui.mesh.editMaterial
            ;

    blocker.block();

    tbe::Vector2b billboard = m_target->getBillBoard();
    m_mainwin->nodesGui.mesh.billboardX->setChecked(billboard.x);
    m_mainwin->nodesGui.mesh.billboardY->setChecked(billboard.y);

    m_mainwin->nodesGui.mesh.castshadow->setChecked(m_target->isCastShadow());
    m_mainwin->nodesGui.mesh.receiveshadow->setChecked(m_target->isReceiveShadow());

    string matFile = m_target->getMaterialFile();

    if(!matFile.empty())
    {
        if(!m_materialDialog)
            m_materialDialog = new MaterialDialog(m_mainwin, m_target, matFile.c_str());

        m_materialDialog->update();

        m_mainwin->nodesGui.mesh.reloadMaterial->setEnabled(true);
        m_mainwin->nodesGui.mesh.releaseMaterial->setEnabled(true);

        m_mainwin->nodesGui.mesh.matinfo->setText(matFile.c_str());
    }
    else
    {
        m_mainwin->nodesGui.mesh.reloadMaterial->setEnabled(false);
        m_mainwin->nodesGui.mesh.releaseMaterial->setEnabled(false);

        m_mainwin->nodesGui.mesh.matinfo->clear();
    }

    blocker.unblock();

    m_mainwin->m_tbeWidget->highlight(this);
}
