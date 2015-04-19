/* 
 * File:   EnvironmentDialog.cpp
 * Author: b4n92uid
 * 
 * Created on 23 mai 2013, 17:14
 */

#include "EnvironmentDialog.h"

#include "QVectorBox.h"
#include "MainWindow.h"

using namespace std;
using namespace tbe;
using namespace scene;

EnvironmentDialog::EnvironmentDialog(MainWindow* parent) : QDialog(parent)
{
    m_mainwin = parent;
    m_tbeWidget = parent->tbeWidget();

    setupUi(this);

    sceneAmbiant = new QDoubleVector3Box(this, ambient_x, ambient_y, ambient_z);
    fog_color = new QDoubleVector3Box(this, fog_x, fog_y, fog_z);
}

EnvironmentDialog::~EnvironmentDialog() { }

void EnvironmentDialog::initConnection()
{
    connect(sceneAmbiant, SIGNAL(valueChanged(const tbe::Vector3f&)), this, SLOT(guiAmbient(const tbe::Vector3f&)));

    connect(znear, SIGNAL(valueChanged(double)), this, SLOT(guiZNear(double)));
    connect(zfar, SIGNAL(valueChanged(double)), this, SLOT(guiZFar(double)));

    connect(fog_enable, SIGNAL(clicked(bool)), this, SLOT(guiFogEnable(bool)));
    connect(fog_color, SIGNAL(valueChanged(const tbe::Vector3f&)), this, SLOT(guiFogChange()));
    connect(fog_start, SIGNAL(valueChanged(double)), this, SLOT(guiFogChange()));
    connect(fog_end, SIGNAL(valueChanged(double)), this, SLOT(guiFogChange()));

    connect(skybox_enable, SIGNAL(clicked(bool)), this, SLOT(guiSkyboxEnable(bool)));
    connect(skybox_apply, SIGNAL(clicked()), this, SLOT(guiSkyboxChange()));
    connect(skybox_browse, SIGNAL(clicked()), this, SLOT(guiSkyboxBrowse()));
    connect(skybox_up, SIGNAL(clicked()), this, SLOT(guiSkyboxShift()));
    connect(skybox_down, SIGNAL(clicked()), this, SLOT(guiSkyboxShift()));
}

void EnvironmentDialog::updateInterface()
{
    SceneManager* sceneManager = m_tbeWidget->rootNode()->getSceneManager();

    zfar->setValue(sceneManager->getZFar());
    znear->setValue(sceneManager->getZNear());

    Fog* fog = sceneManager->getFog();

    if(fog->isEnable())
    {
        QSignalBlockerStream blocker;
        blocker << fog_enable << fog_color << fog_start << fog_end;
        blocker.block();

        fog_enable->setChecked(fog->isEnable());
        fog_color->setValue(math::vec43(fog->getColor()));
        fog_start->setValue(fog->getStart());
        fog_end->setValue(fog->getEnd());

        blocker.unblock();
    }

    SkyBox* sky = sceneManager->getSkybox();

    if(sky->isEnable())
    {
        QSignalBlockerStream blocker;
        blocker << skybox_apply << skybox_enable;
        blocker.block();

        blocker.block();

        tbe::Texture* texs = sky->getTextures();

        skybox_list->clear();

        QMap<QString, QString> skymap;
        skymap["1:Devant"] = QString::fromStdString(texs[0].getFilename());
        skymap["2:Dèrrier"] = QString::fromStdString(texs[1].getFilename());
        skymap["3:Haut"] = QString::fromStdString(texs[2].getFilename());
        skymap["4:Bas"] = QString::fromStdString(texs[3].getFilename());
        skymap["5:Gauche"] = QString::fromStdString(texs[4].getFilename());
        skymap["6:Droite"] = QString::fromStdString(texs[5].getFilename());

        foreach(QString k, skymap.keys())
        {
            QString path = skymap.value(k);

            QTreeWidgetItem* item = new QTreeWidgetItem(QStringList() << k << QFileInfo(path).baseName());
            item->setData(1, Qt::UserRole, path);

            skybox_list->addTopLevelItem(item);
        }

        skybox_enable->setChecked(sky->isEnable());

        blocker.unblock();
    }

    {
        sceneAmbiant->blockSignals(true);
        sceneAmbiant->setValue(math::vec43(sceneManager->getAmbientLight()));
        sceneAmbiant->blockSignals(false);
    }

}

void EnvironmentDialog::guiAmbient(const tbe::Vector3f& value)
{
    m_tbeWidget->setSceneAmbiant(value);

    m_mainwin->notifyChange(true);
}

void EnvironmentDialog::guiSkyboxEnable(bool enable)
{
    m_tbeWidget->setSkybox(enable);

    m_mainwin->notifyChange(true);
}

void EnvironmentDialog::guiSkyboxBrowse()
{
    QStringList files = QFileDialog::getOpenFileNames(this);

    if(files.size() != 6)
    {
        QMessageBox::warning(this, "Skybox", "Vous devez ajouter 6 fichiers !");
        return;
    }

    skybox_list->clear();

    QTreeWidgetItem* item;

    item = new QTreeWidgetItem(QStringList() << "1:Devant" << QFileInfo(files[0]).baseName());
    item->setData(1, Qt::UserRole, files[0]);
    skybox_list->addTopLevelItem(item);

    item = new QTreeWidgetItem(QStringList() << "2:Dèrriere" << QFileInfo(files[1]).baseName());
    item->setData(1, Qt::UserRole, files[1]);
    skybox_list->addTopLevelItem(item);

    item = new QTreeWidgetItem(QStringList() << "3:Haut" << QFileInfo(files[2]).baseName());
    item->setData(1, Qt::UserRole, files[2]);
    skybox_list->addTopLevelItem(item);

    item = new QTreeWidgetItem(QStringList() << "4:Bas" << QFileInfo(files[3]).baseName());
    item->setData(1, Qt::UserRole, files[3]);
    skybox_list->addTopLevelItem(item);

    item = new QTreeWidgetItem(QStringList() << "5:Gauche" << QFileInfo(files[4]).baseName());
    item->setData(1, Qt::UserRole, files[4]);
    skybox_list->addTopLevelItem(item);

    item = new QTreeWidgetItem(QStringList() << "6:Droite" << QFileInfo(files[5]).baseName());
    item->setData(1, Qt::UserRole, files[5]);
    skybox_list->addTopLevelItem(item);
}

void EnvironmentDialog::guiSkyboxShift()
{
    if(skybox_list->selectedItems().empty())
        return;

    QTreeWidgetItem *itemSrc, *itemDst;

    itemSrc = skybox_list->selectedItems().front();

    if(sender() == skybox_up)
        itemDst = skybox_list->itemAbove(itemSrc);

    else if(sender() == skybox_down)
        itemDst = skybox_list->itemBelow(itemSrc);

    if(!itemDst)
        return;

    QString src = itemSrc->data(1, Qt::UserRole).toString();
    QString dst = itemDst->data(1, Qt::UserRole).toString();

    itemSrc->setData(1, Qt::UserRole, dst);
    itemSrc->setText(1, QFileInfo(dst).baseName());

    itemDst->setData(1, Qt::UserRole, src);
    itemDst->setText(1, QFileInfo(src).baseName());

    skybox_list->setCurrentItem(itemDst);
}

void EnvironmentDialog::guiSkyboxChange()
{
    if(skybox_list->topLevelItemCount() != 6)
        return;

    QStringList texs;

    for(unsigned i = 0; i < 6; i++)
        texs << skybox_list->topLevelItem(i)->data(1, Qt::UserRole).toString();

    try
    {
        m_tbeWidget->setSkybox(texs);
        m_tbeWidget->setSkybox(true);
    }
    catch(std::exception& e)
    {
        QMessageBox::critical(this, "Erreur: Skybox", e.what());
    }

    m_mainwin->notifyChange(true);
}

void EnvironmentDialog::guiFogEnable(bool enable)
{
    m_tbeWidget->setFog(enable);


    m_mainwin->notifyChange(true);
}

void EnvironmentDialog::guiFogChange()
{
    m_tbeWidget->setFog(tbe::math::vec34(fog_color->value()), fog_start->value(), fog_end->value());

    m_mainwin->notifyChange(true);
}

void EnvironmentDialog::guiZNear(double value)
{
    m_tbeWidget->setZNear(value);
}

void EnvironmentDialog::guiZFar(double value)
{
    m_tbeWidget->setZFar(value);
}
