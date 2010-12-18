/* 
 * File:   QEnvManager.cpp
 * Author: b4n92uid
 * 
 * Created on 4 décembre 2010, 00:04
 */

#include "QEnvManager.h"
#include "QDebug"

QEnvManager::QEnvManager(QWidget* parent, Ui_mainWindow* uinterface) : QObject(parent)
{
    m_parent = parent;

    m_sceneAmbiant = new QVectorBox(this, uinterface->env_ambient_x, uinterface->env_ambient_y, uinterface->env_ambient_z);
    connect(m_sceneAmbiant, SIGNAL(valueChanged(const tbe::Vector3f&)), this, SIGNAL(sceneAmbiantUpdate(const tbe::Vector3f&)));

    skybox.apply = uinterface->skybox_apply;
    skybox.enable = uinterface->skybox_enable;

    skybox.textures[0] = new QBrowsEdit(this, uinterface->skybox_front, uinterface->skybox_front_browse);
    skybox.textures[1] = new QBrowsEdit(this, uinterface->skybox_back, uinterface->skybox_back_browse);
    skybox.textures[2] = new QBrowsEdit(this, uinterface->skybox_left, uinterface->skybox_left_browse);
    skybox.textures[3] = new QBrowsEdit(this, uinterface->skybox_right, uinterface->skybox_right_browse);
    skybox.textures[4] = new QBrowsEdit(this, uinterface->skybox_top, uinterface->skybox_top_browse);
    skybox.textures[5] = new QBrowsEdit(this, uinterface->skybox_bottom, uinterface->skybox_bottom_browse);

    fog.color = new QVectorBox(this, uinterface->fog_x, uinterface->fog_y, uinterface->fog_z);
    fog.start = uinterface->fog_start;
    fog.end = uinterface->fog_end;
    fog.enable = uinterface->fog_enable;
}

QEnvManager::~QEnvManager()
{
}

void QEnvManager::skyboxChanged(bool stat)
{
    if(stat)
    {
        QStringList texs;

        for(unsigned i = 0; i < 6; i++)
        {
            texs << skybox.textures[i]->getOpenFileName();

            if(texs.back().isEmpty())
            {
                QMessageBox::warning(m_parent, "Attention", "Les textures du skybox doivent être toute spécifier");
                return;
            }
        }

        emit skyboxApply(texs);
    }

    else
    {
        emit skyboxClear();
    }
}

void QEnvManager::fogChanged(bool stat)
{
    if(stat)
    {
        emit fogApply(vec34(fog.color->value()),
                      (float)fog.start->value(),
                      (float)fog.end->value());
    }
    else
    {
        emit fogClear();
    }
}

void QEnvManager::fogInit(tbe::scene::Fog* tbefog)
{
    this->fog.enable->setChecked(tbefog->IsEnable());
    this->fog.color->setValue(vec43(tbefog->GetColor()));
    this->fog.start->setValue(tbefog->GetStart());
    this->fog.end->setValue(tbefog->GetEnd());

    connect(fog.enable, SIGNAL(clicked(bool)), this, SLOT(fogChanged(bool)));
    connect(fog.color, SIGNAL(valueChanged(const tbe::Vector3f&)), this, SLOT(fogChanged()));
    connect(fog.start, SIGNAL(valueChanged(double)), this, SLOT(fogChanged()));
    connect(fog.end, SIGNAL(valueChanged(double)), this, SLOT(fogChanged()));
}

void QEnvManager::skyboxInit(tbe::scene::SkyBox* tbesky)
{
    tbe::Texture* texs = tbesky->GetTextures();

    for(unsigned i = 0; i < 6; i++)
        this->skybox.textures[i]->setOpenFileName(texs[i].GetFilename().c_str());

    this->skybox.enable->setChecked(tbesky->IsEnable());

    connect(skybox.enable, SIGNAL(clicked(bool)), this, SLOT(skyboxChanged(bool)));
    connect(skybox.apply, SIGNAL(clicked()), this, SLOT(skyboxChanged()));
}

void QEnvManager::ambiantInit(const tbe::Vector3f& value)
{
    m_sceneAmbiant->setValue(value);
}
