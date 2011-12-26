/* 
 * File:   QLightInteractor.cpp
 * Author: b4n92uid
 * 
 * Created on 29 août 2011, 07:46
 */

#include "QLightInteractor.h"
#include "MainWindow.h"

QLightInteractor::QLightInteractor(MainWindow* mainwin, tbe::scene::Light* target)
: QNodeInteractor(mainwin, target), m_target(target)
{

}

QLightInteractor::~QLightInteractor()
{
}

QString QLightInteractor::typeName() const
{
    return "Light";
}

void QLightInteractor::setType(int type)
{
    m_target->setType((tbe::scene::Light::Type)type);
}

void QLightInteractor::setAmbiant(const tbe::Vector3f& value)
{
    m_target->setAmbient(tbe::math::vec34(value));
}

void QLightInteractor::setDiffuse(const tbe::Vector3f& value)
{
    m_target->setDiffuse(tbe::math::vec34(value));
}

void QLightInteractor::setSpecular(const tbe::Vector3f& value)
{
    m_target->setSpecular(tbe::math::vec34(value));
}

void QLightInteractor::setRadius(double value)
{
    m_target->setRadius((float)value);
}

void QLightInteractor::select()
{
    deselect();

    QNodeInteractor::select();

    connect(m_mainwin->nodesGui.light.type, SIGNAL(activated(int)), this, SLOT(setType(int)));
    connect(m_mainwin->nodesGui.light.ambiant, SIGNAL(valueChanged(const tbe::Vector3f&)), this, SLOT(setAmbiant(const tbe::Vector3f&)));
    connect(m_mainwin->nodesGui.light.diffuse, SIGNAL(valueChanged(const tbe::Vector3f&)), this, SLOT(setDiffuse(const tbe::Vector3f&)));
    connect(m_mainwin->nodesGui.light.specular, SIGNAL(valueChanged(const tbe::Vector3f&)), this, SLOT(setSpecular(const tbe::Vector3f&)));
    connect(m_mainwin->nodesGui.light.radius, SIGNAL(valueChanged(double)), this, SLOT(setRadius(double)));

    update();

    m_mainwin->nodesGui.attribTab->setCurrentIndex(1);
}

void QLightInteractor::deselect()
{
    QNodeInteractor::deselect();

    disconnect(m_mainwin->nodesGui.light.type, SIGNAL(activated(int)), 0, 0);
    disconnect(m_mainwin->nodesGui.light.ambiant, SIGNAL(valueChanged(const tbe::Vector3f&)), 0, 0);
    disconnect(m_mainwin->nodesGui.light.diffuse, SIGNAL(valueChanged(const tbe::Vector3f&)), 0, 0);
    disconnect(m_mainwin->nodesGui.light.specular, SIGNAL(valueChanged(const tbe::Vector3f&)), 0, 0);
    disconnect(m_mainwin->nodesGui.light.radius, SIGNAL(valueChanged(double)), 0, 0);
}

void QLightInteractor::update()
{
    QNodeInteractor::update();

    QSignalBlocker blocker;
    blocker << m_mainwin->nodesGui.light.type << m_mainwin->nodesGui.light.ambiant
            << m_mainwin->nodesGui.light.diffuse << m_mainwin->nodesGui.light.specular
            << m_mainwin->nodesGui.light.radius;

    blocker.block();

    m_mainwin->nodesGui.light.type->setCurrentIndex((int)m_target->getType());
    m_mainwin->nodesGui.light.ambiant->setValue(tbe::math::vec43(m_target->getAmbient()));
    m_mainwin->nodesGui.light.diffuse->setValue(tbe::math::vec43(m_target->getDiffuse()));
    m_mainwin->nodesGui.light.specular->setValue(tbe::math::vec43(m_target->getSpecular()));
    m_mainwin->nodesGui.light.radius->setValue(m_target->getRadius());

    blocker.unblock();

    m_mainwin->m_tbeWidget->selBox()->setAround(m_target, tbe::Vector4f(1, 1, 1, 0.25));
}
