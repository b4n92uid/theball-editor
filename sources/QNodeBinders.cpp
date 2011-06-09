/*
 * File:   QNodeBinders.cpp
 * Author: b4n92uid
 *
 * Created on 7 décembre 2010, 21:00
 */

#include "QNodeBinders.h"
#include <QMessageBox>

QNodeBinders::QNodeBinders(QWidget* parent) : QObject(parent)
{
    m_parentWidget = parent;

    m_curmesh = NULL;
    m_curmark = NULL;
    m_curparticles = NULL;
    m_curwater = NULL;
    m_curlight = NULL;
    m_curNode = NULL;
}

void QNodeBinders::node(tbe::scene::Node* curNode)
{
    m_curNode = curNode;
    m_curmesh = NULL;
    m_curmark = NULL;
    m_curparticles = NULL;
    m_curwater = NULL;
    m_curlight = NULL;
}

tbe::scene::Node* QNodeBinders::node() const
{
    return m_curNode;
}

void QNodeBinders::mesh(tbe::scene::Mesh* curmesh)
{
    m_curNode = m_curmesh = curmesh;
    m_curwater = NULL;
    m_curparticles = NULL;
    m_curlight = NULL;
    m_curmark = NULL;
}

tbe::scene::Mesh* QNodeBinders::mesh() const
{
    return m_curmesh;
}

void QNodeBinders::light(tbe::scene::Light* curlight)
{
    m_curNode = m_curlight = curlight;
    m_curmesh = NULL;
    m_curparticles = NULL;
    m_curwater = NULL;
    m_curmark = NULL;
}

tbe::scene::Light* QNodeBinders::light() const
{
    return m_curlight;
}

void QNodeBinders::water(tbe::scene::Water* curwater)
{
    m_curNode = m_curwater = curwater;
    m_curmesh = NULL;
    m_curparticles = NULL;
    m_curlight = NULL;
    m_curmark = NULL;
}

tbe::scene::Water* QNodeBinders::water() const
{
    return m_curwater;
}

void QNodeBinders::particles(tbe::scene::ParticlesEmiter* curparticles)
{
    m_curNode = m_curparticles = curparticles;
    m_curmesh = NULL;
    m_curwater = NULL;
    m_curlight = NULL;
    m_curmark = NULL;
}

void QNodeBinders::mark(tbe::scene::MapMark* curNode)
{
    m_curNode = m_curmark = curNode;
    m_curparticles = NULL;
    m_curmesh = NULL;
    m_curwater = NULL;
    m_curlight = NULL;
}

tbe::scene::MapMark* QNodeBinders::mark() const
{
    return m_curmark;
}

tbe::scene::ParticlesEmiter* QNodeBinders::particles() const
{
    return m_curparticles;
}

void QNodeBinders::nodeSetName(const QString& s)
{
    if(m_curNode)
    {
        m_curNode->setName(s.toStdString());
        emit notifyUpdate(m_curNode);
    }
}

void QNodeBinders::nodeSetPos(const tbe::Vector3f& v)
{
    if(m_curNode)
    {
        m_curNode->setPos(v);
        emit notifyUpdate(m_curNode);
    }
}

void QNodeBinders::nodeSetRotation(const tbe::Vector3f& v)
{
    if(m_curNode)
    {
        m_curNode->getMatrix().setRotate(tbe::Quaternion(v));
        emit notifyUpdate(m_curNode);
    }
}

void QNodeBinders::nodeSetScale(const tbe::Vector3f& v)
{
    if(m_curNode)
    {
        m_curNode->getMatrix().setScale(v);
        emit notifyUpdate(m_curNode);
    }
}

void QNodeBinders::nodeSetMatrix(const tbe::Matrix4& m)
{
    if(m_curNode)
    {
        m_curNode->setMatrix(m);
        emit notifyUpdate(m_curNode);
    }
}

void QNodeBinders::nodeSetEnalbe(bool stat)
{
    node()->setEnable(stat);
}

void QNodeBinders::waterSetDeform(double v)
{
    if(m_curwater)
    {
        m_curwater->setDeform(v);
        emit notifyUpdate(m_curwater);
    }
}

void QNodeBinders::waterSetSize(const tbe::Vector2f& v)
{
    if(m_curwater)
    {
        m_curwater->setSize(v);
        emit notifyUpdate(m_curwater);
    }
}

void QNodeBinders::waterSetUvRepeat(const tbe::Vector2f& v)
{
    if(m_curwater)
    {
        m_curwater->setUvRepeat(v);
        emit notifyUpdate(m_curwater);
    }
}

void QNodeBinders::waterSetSpeed(double v)
{
    if(m_curwater)
    {
        m_curwater->setSpeed(v);
        emit notifyUpdate(m_curwater);
    }
}

void QNodeBinders::waterSetBlend(double v)
{
    if(m_curwater)
    {
        m_curwater->setBlend(v);
        emit notifyUpdate(m_curwater);
    }
}

void QNodeBinders::particleSetGravity(const tbe::Vector3f& v)
{
    if(m_curparticles)
    {
        m_curparticles->setGravity(v);
        emit notifyUpdate(m_curparticles);
    }
}

void QNodeBinders::particleSetBoxsize(const tbe::Vector3f& v)
{
    if(m_curparticles)
    {
        m_curparticles->setBoxSize(v);
        emit notifyUpdate(m_curparticles);
    }
}

void QNodeBinders::particleSetFreemove(double v)
{
    if(m_curparticles)
    {
        m_curparticles->setFreeMove(v);
        emit notifyUpdate(m_curparticles);
    }
}

void QNodeBinders::particleSetLifeinit(double v)
{
    if(m_curparticles)
    {
        m_curparticles->setLifeInit(v);
        emit notifyUpdate(m_curparticles);
    }
}

void QNodeBinders::particleSetLifedown(double v)
{
    if(m_curparticles)
    {
        m_curparticles->setLifeDown(v);
        emit notifyUpdate(m_curparticles);
    }
}

void QNodeBinders::particleSetNumber(int v)
{
    if(m_curparticles)
    {
        m_curparticles->setNumber(v);
        emit notifyUpdate(m_curparticles);
    }
}

void QNodeBinders::particleSetTexture(const QString& v)
{
    if(m_curparticles && !v.isEmpty())
    {
        try
        {
            m_curparticles->setTexture(v.toStdString());
            emit notifyUpdate(m_curparticles);
        }
        catch(std::exception& e)
        {
            QMessageBox::critical(m_parentWidget, "Chargement de texture", e.what());
        }
    }

}

void QNodeBinders::particleSetContinousMode(int v)
{
    if(m_curparticles)
    {
        m_curparticles->setContinousMode(v);
        emit notifyUpdate(m_curparticles);
    }
}

void QNodeBinders::particleBuild()
{
    if(m_curparticles)
    {
        m_curparticles->build();
        emit notifyUpdate(m_curparticles);
    }
}

void QNodeBinders::lightSetType(int type)
{
    if(m_curlight)
    {
        m_curlight->setType((tbe::scene::Light::Type)type);
        emit notifyUpdate(m_curlight);
    }
}

void QNodeBinders::lightSetAmbiant(const tbe::Vector3f& value)
{
    if(m_curlight)
    {
        m_curlight->setAmbient(vec34(value));
        emit notifyUpdate(m_curlight);
    }
}

void QNodeBinders::lightSetDiffuse(const tbe::Vector3f& value)
{
    if(m_curlight)
    {
        m_curlight->setDiffuse(vec34(value));
        emit notifyUpdate(m_curlight);
    }
}

void QNodeBinders::lightSetSpecular(const tbe::Vector3f& value)
{
    if(m_curlight)
    {
        m_curlight->setSpecular(vec34(value));
        emit notifyUpdate(m_curlight);
    }
}

void QNodeBinders::lightSetRadius(double value)
{
    if(m_curlight)
    {
        m_curlight->setRadius((float)value);
        emit notifyUpdate(m_curlight);
    }
}
