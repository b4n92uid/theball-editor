/* 
 * File:   MeshDialog.cpp
 * Author: b4n92uid
 * 
 * Created on 23 mai 2013, 17:09
 */

#include "MeshDialog.h"
#include "QVectorBox.h"
#include "QMeshInteractor.h"
#include "QSignalBlocker.h"
#include "MaterialDialog.h"
#include "MainWindow.h"

MeshDialog::MeshDialog(MainWindow* parent) : QDialog(parent), m_mainwin(parent)
{
    setupUi(this);

    m_materialDialog = new MaterialDialog(m_mainwin);
}

MeshDialog::~MeshDialog() { }

void MeshDialog::bind(QMeshInteractor* mi)
{
    connect(editMaterial, SIGNAL(clicked()), this, SLOT(onOpenMaterialDialog()));
    connect(attachMaterial, SIGNAL(clicked()), this, SLOT(onAttachMaterial()));
    connect(releaseMaterial, SIGNAL(clicked()), this, SLOT(onReleaseMaterial()));

    connect(this, SIGNAL(attachMaterialFile(QString)), mi, SLOT(attachMaterial(QString)));
    connect(reloadMaterial, SIGNAL(clicked()), mi, SLOT(reloadMaterial()));
    connect(this, SIGNAL(releaseMaterialFile()), mi, SLOT(releaseMaterial()));

    connect(billboardX, SIGNAL(clicked(bool)), mi, SLOT(setBillBoardX(bool)));
    connect(billboardY, SIGNAL(clicked(bool)), mi, SLOT(setBillBoardY(bool)));

    connect(castshadow, SIGNAL(clicked(bool)), mi, SLOT(setCastShadow(bool)));
    connect(receiveshadow, SIGNAL(clicked(bool)), mi, SLOT(setReceiveShadow(bool)));

    connect(computeNormal, SIGNAL(clicked()), mi, SLOT(setComputeNormal()));
    connect(computeTangent, SIGNAL(clicked()), mi, SLOT(setComputeTangent()));
}

void MeshDialog::unbind()
{
    disconnect(editMaterial, SIGNAL(clicked()), 0, 0);
    disconnect(attachMaterial, SIGNAL(clicked()), 0, 0);
    disconnect(releaseMaterial, SIGNAL(clicked()), 0, 0);
    disconnect(reloadMaterial, SIGNAL(clicked()), 0, 0);

    disconnect(billboardX, SIGNAL(clicked()), 0, 0);
    disconnect(billboardY, SIGNAL(clicked()), 0, 0);

    disconnect(castshadow, SIGNAL(clicked()), 0, 0);
    disconnect(receiveshadow, SIGNAL(clicked()), 0, 0);
    disconnect(computeNormal, SIGNAL(clicked()), 0, 0);
    disconnect(computeTangent, SIGNAL(clicked()), 0, 0);

    billboardX->setChecked(false);
    billboardY->setChecked(false);
    castshadow->setChecked(false);
    receiveshadow->setChecked(false);
    computeNormal->setChecked(false);
    computeTangent->setChecked(false);
}

void MeshDialog::update(tbe::scene::Mesh* m)
{
    QSignalBlocker blocker;
    blocker << billboardX << billboardY << castshadow << receiveshadow << editMaterial;

    blocker.block();

    tbe::Vector2b billboard = m->getBillBoard();
    billboardX->setChecked(billboard.x);
    billboardY->setChecked(billboard.y);

    castshadow->setChecked(m->isCastShadow());
    receiveshadow->setChecked(m->isReceiveShadow());
    computeNormal->setChecked(m->isComputeNormals());
    computeTangent->setChecked(m->isComputeTangent());

    std::string matFile = m->getMaterialFile();

    if(!matFile.empty())
    {
        QString path = QString::fromStdString(matFile);
        m_materialDialog->update(m, path);
        reloadMaterial->setEnabled(true);
        releaseMaterial->setEnabled(true);
        matinfo->setText(path);
    }

    blocker.unblock();
}

void MeshDialog::onOpenMaterialDialog()
{
    using namespace std;

    m_materialDialog->bind();
    m_materialDialog->show();
}

void MeshDialog::onAttachMaterial()
{
    QString filename = QFileDialog::getOpenFileName(m_mainwin, "", m_mainwin->openFileName(),
                                                    "Material (*.material);;Tout les fichiers (*.*)");
    if(!filename.isEmpty())
    {
        reloadMaterial->setEnabled(true);
        releaseMaterial->setEnabled(true);

        matinfo->setText(filename);

        emit attachMaterialFile(filename);
    }
}

void MeshDialog::onReleaseMaterial()
{
    reloadMaterial->setEnabled(false);
    releaseMaterial->setEnabled(false);
    matinfo->clear();

    emit releaseMaterialFile();
}
