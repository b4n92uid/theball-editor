/* 
 * File:   ParticlesDialog.cpp
 * Author: b4n92uid
 * 
 * Created on 23 mai 2013, 17:09
 */

#include "ParticlesDialog.h"

#include "QParticlesInteractor.h"
#include "QBrowsEdit.h"
#include "QVectorBox.h"
#include "QSignalBlocker.h"
#include "MainWindow.h"

ParticlesDialog::ParticlesDialog(MainWindow* parent) : QDialog(parent), m_mainwin(parent)
{
    setupUi(this);

    gravity = new QDoubleVector3Box(this, gravity_x, gravity_y, gravity_z);
    boxsize = new QDoubleVector3Box(this, boxsize_x, boxsize_y, boxsize_z);
    bulletsize = new QDoubleVector2Box(this, bulletsize_x, bulletsize_y);
    texture = new QBrowsEdit(m_mainwin, texture_path, texture_browse);
}

ParticlesDialog::~ParticlesDialog() { }

void ParticlesDialog::bind(QParticlesInteractor* li)
{
    connect(gravity, SIGNAL(valueChanged(const tbe::Vector3f&)), li, SLOT(setGravity(const tbe::Vector3f&)));
    connect(boxsize, SIGNAL(valueChanged(const tbe::Vector3f&)), li, SLOT(setBoxsize(const tbe::Vector3f&)));
    connect(bulletsize, SIGNAL(valueChanged(const tbe::Vector2f&)), li, SLOT(setBulletsize(const tbe::Vector2f&)));
    connect(freemove, SIGNAL(valueChanged(double)), li, SLOT(setFreemove(double)));
    connect(lifeinit, SIGNAL(valueChanged(double)), li, SLOT(setLifeinit(double)));
    connect(lifedown, SIGNAL(valueChanged(double)), li, SLOT(setLifedown(double)));
    connect(number, SIGNAL(valueChanged(int)), li, SLOT(setNumber(int)));
    connect(texture, SIGNAL(textChanged(const QString&)), li, SLOT(setTexture(const QString&)));
    connect(continuousmode, SIGNAL(clicked(bool)), li, SLOT(setContinousMode(bool)));
    connect(pointsprite, SIGNAL(clicked(bool)), li, SLOT(setPointSprite(bool)));
    connect(build, SIGNAL(clicked()), li, SLOT(build()));
}

void ParticlesDialog::unbind()
{
    disconnect(gravity, SIGNAL(valueChanged(const tbe::Vector3f&)), 0, 0);
    disconnect(boxsize, SIGNAL(valueChanged(const tbe::Vector3f&)), 0, 0);
    disconnect(bulletsize, SIGNAL(valueChanged(const tbe::Vector2f&)), 0, 0);
    disconnect(freemove, SIGNAL(valueChanged(double)), 0, 0);
    disconnect(lifeinit, SIGNAL(valueChanged(double)), 0, 0);
    disconnect(lifedown, SIGNAL(valueChanged(double)), 0, 0);
    disconnect(number, SIGNAL(valueChanged(int)), 0, 0);
    disconnect(texture, SIGNAL(textChanged(const QString&)), 0, 0);
    disconnect(continuousmode, SIGNAL(clicked(bool)), 0, 0);
    disconnect(pointsprite, SIGNAL(clicked(bool)), 0, 0);
    disconnect(build, SIGNAL(clicked()), 0, 0);
}

void ParticlesDialog::update(tbe::scene::ParticlesEmiter* l)
{

    QSignalBlocker blocker;
    blocker << gravity << boxsize
            << bulletsize << freemove
            << lifeinit << lifedown
            << number << texture
            << continuousmode << pointsprite;

    blocker.block();

    gravity->setValue(l->getGravity());
    boxsize->setValue(l->getBoxSize());
    bulletsize->setValue(l->getBulletSize());
    freemove->setValue(l->getFreeMove());
    lifeinit->setValue(l->getLifeInit());
    lifedown->setValue(l->getLifeDown());
    number->setValue(l->getNumber());
    texture->setOpenFileName(QString::fromStdString(l->getTexture().getFilename()));
    continuousmode->setChecked(l->isContinousMode());
    pointsprite->setChecked(l->isUsePointSprite());

    blocker.unblock();
}