/* 
 * File:   LightDialog.cpp
 * Author: b4n92uid
 * 
 * Created on 23 mai 2013, 17:08
 */

#include "LightDialog.h"

#include "QLightInteractor.h"
#include "QVectorBox.h"
#include "QSignalBlocker.h"
#include "QBrowsEdit.h"
#include "MainWindow.h"

LightDialog::LightDialog(MainWindow* parent) : QDialog(parent), m_mainwin(parent)
{
    setupUi(this);

    ambiant = new QDoubleVector3Box(this, ambiant_x, ambiant_y, ambiant_z);
    diffuse = new QDoubleVector3Box(this, diffuse_x, diffuse_y, diffuse_z);
    specular = new QDoubleVector3Box(this, specular_x, specular_y, specular_z);
    rays_offset = new QDoubleVector3Box(this, rays_offset_x, rays_offset_y, rays_offset_z);
    rays_noise = new QBrowsEdit(m_mainwin, rays_noise_path, rays_noise_browse);
}

LightDialog::~LightDialog() { }

void LightDialog::bind(QLightInteractor* li)
{
    connect(type, SIGNAL(activated(int)), li, SLOT(setType(int)));
    connect(ambiant, SIGNAL(valueChanged(const tbe::Vector3f&)), li, SLOT(setAmbiant(const tbe::Vector3f&)));
    connect(diffuse, SIGNAL(valueChanged(const tbe::Vector3f&)), li, SLOT(setDiffuse(const tbe::Vector3f&)));
    connect(specular, SIGNAL(valueChanged(const tbe::Vector3f&)), li, SLOT(setSpecular(const tbe::Vector3f&)));
    connect(radius, SIGNAL(valueChanged(double)), li, SLOT(setRadius(double)));

    connect(castshadow, SIGNAL(clicked(bool)), li, SLOT(setCastShadow(bool)));
    connect(shadow_size, SIGNAL(valueChanged(int)), li, SLOT(setShadowSize(int)));
    connect(shadow_blur, SIGNAL(valueChanged(int)), li, SLOT(setShadowBlur(int)));
    connect(shadow_intensity, SIGNAL(valueChanged(double)), li, SLOT(setShadowIntensity(double)));
    connect(shadow_shader, SIGNAL(clicked(bool)), li, SLOT(setShadowShader(bool)));

    connect(castrays, SIGNAL(clicked(bool)), li, SLOT(setCastRays(bool)));
    connect(rays_offset, SIGNAL(valueChanged(const tbe::Vector3f&)), li, SLOT(setRaysOffset(const tbe::Vector3f&)));
    connect(rays_framesize, SIGNAL(valueChanged(int)), li, SLOT(setRaysFrameSize(int)));
    connect(rays_lightsize, SIGNAL(valueChanged(int)), li, SLOT(setRaysLightSize(int)));
    connect(rays_noise, SIGNAL(textChanged(const QString&)), li, SLOT(setRaysNoiseLayer(QString)));

}

void LightDialog::unbind()
{
    disconnect(type, SIGNAL(activated(int)), 0, 0);
    disconnect(ambiant, SIGNAL(valueChanged(const tbe::Vector3f&)), 0, 0);
    disconnect(diffuse, SIGNAL(valueChanged(const tbe::Vector3f&)), 0, 0);
    disconnect(specular, SIGNAL(valueChanged(const tbe::Vector3f&)), 0, 0);
    disconnect(radius, SIGNAL(valueChanged(double)), 0, 0);

    disconnect(castshadow, SIGNAL(clicked(bool)), 0, 0);
    disconnect(shadow_size, SIGNAL(valueChanged(int)), 0, 0);
    disconnect(shadow_blur, SIGNAL(valueChanged(int)), 0, 0);
    disconnect(shadow_intensity, SIGNAL(valueChanged(double)), 0, 0);
    disconnect(shadow_shader, SIGNAL(clicked(bool)), 0, 0);

    disconnect(castrays, SIGNAL(clicked(bool)), 0, 0);
    disconnect(rays_offset, SIGNAL(valueChanged(const tbe::Vector3f&)), 0, 0);
    disconnect(rays_framesize, SIGNAL(valueChanged(int)), 0, 0);
    disconnect(rays_lightsize, SIGNAL(valueChanged(int)), 0, 0);
    disconnect(rays_noise, SIGNAL(textChanged(const QString&)), 0, 0);
}

void LightDialog::update(tbe::scene::Light* l)
{
    QSignalBlockerStream blocker;
    blocker << type << ambiant << diffuse << specular << radius
            << castshadow << shadow_size << shadow_blur << shadow_intensity << shadow_shader
            << castrays << rays_offset << rays_framesize << rays_lightsize << rays_noise;

    blocker.block();

    type->setCurrentIndex((int) l->getType());
    ambiant->setValue(tbe::math::vec43(l->getAmbient()));
    diffuse->setValue(tbe::math::vec43(l->getDiffuse()));
    specular->setValue(tbe::math::vec43(l->getSpecular()));
    radius->setValue(l->getRadius());

    castshadow->setChecked(l->isCastShadow());
    castrays->setChecked(l->isCastRays());

    tbe::scene::ShadowMap* smap = l->getShadowMap();

    if(smap)
    {
        shadow_size->setValue(smap->getFrameSize().x);
        shadow_blur->setValue(smap->getBlurPass());
        shadow_intensity->setValue(smap->getIntensity());
        shadow_shader->setChecked(smap->isShaderHandled());
    }

    tbe::scene::VolumetricLight* vlight = l->getVolumeLight();

    if(vlight)
    {
        rays_framesize->setValue(vlight->getFrameSize().x);
        rays_lightsize->setValue(vlight->getLightSize().x);
        rays_noise->setOpenFileName(vlight->getNoiseLayer().getFilename().c_str());
        rays_offset->setValue(vlight->getOffset());
    }

    blocker.unblock();
}
