/* 
 * File:   QNodeBinders.cpp
 * Author: b4n92uid
 * 
 * Created on 7 décembre 2010, 21:00
 */

#include "QNodeBinders.h"

QNodeBinders::QNodeBinders(QObject* parent) : QObject(parent)
{
    m_curmesh = NULL;
    m_curparticles = NULL;
    m_curwater = NULL;
    m_curlight = NULL;
    m_curNode = NULL;
}

void QNodeBinders::setCurNode(tbe::scene::Node* curNode)
{
    m_curNode = curNode;
    m_curmesh = NULL;
    m_curparticles = NULL;
    m_curwater = NULL;
    m_curlight = NULL;

    emit notifyUpdate(m_curNode);
}

tbe::scene::Node* QNodeBinders::getCurNode() const
{
    return m_curNode;
}

void QNodeBinders::setCurmesh(tbe::scene::Mesh* curmesh)
{
    m_curNode = m_curmesh = curmesh;
    m_curwater = NULL;
    m_curparticles = NULL;
    m_curlight = NULL;
}

tbe::scene::Mesh* QNodeBinders::getCurmesh() const
{
    return m_curmesh;
}

void QNodeBinders::setCurlight(tbe::scene::Light* curlight)
{
    m_curNode = m_curlight = curlight;
    m_curmesh = NULL;
    m_curparticles = NULL;
    m_curwater = NULL;
}

tbe::scene::Light* QNodeBinders::getCurlight() const
{
    return m_curlight;
}

void QNodeBinders::setCurwater(tbe::scene::Water* curwater)
{
    m_curNode = m_curwater = curwater;
    m_curmesh = NULL;
    m_curparticles = NULL;
    m_curlight = NULL;

    emit notifyUpdate(m_curwater);
}

tbe::scene::Water* QNodeBinders::getCurwater() const
{
    return m_curwater;
}

void QNodeBinders::setCurparticles(tbe::scene::ParticlesEmiter* curparticles)
{
    m_curNode = m_curparticles = curparticles;
    m_curmesh = NULL;
    m_curwater = NULL;
    m_curlight = NULL;

    emit notifyUpdate(m_curparticles);
}

tbe::scene::ParticlesEmiter* QNodeBinders::getCurparticles() const
{
    return m_curparticles;
}

void QNodeBinders::nodeSetName(const QString& s)
{
    if(m_curNode)
    {
        m_curNode->SetName(s.toStdString());
        emit notifyUpdate(m_curNode);
    }
}

void QNodeBinders::nodeSetPos(const tbe::Vector3f& v)
{
    if(m_curNode)
    {
        m_curNode->SetPos(v);
        emit notifyUpdate(m_curNode);
    }
}

void QNodeBinders::nodeSetMatrix(const tbe::Matrix4f& m)
{
    if(m_curNode)
    {
        m_curNode->SetMatrix(m);
        emit notifyUpdate(m_curNode);
    }
}

void QNodeBinders::waterSetDeform(double v)
{
    if(m_curwater)
    {
        m_curwater->SetDeform(v);
        emit notifyUpdate(m_curwater);
    }
}

void QNodeBinders::waterSetSize(const tbe::Vector2f& v)
{
    if(m_curwater)
    {
        m_curwater->SetSize(v);
        emit notifyUpdate(m_curwater);
    }
}

void QNodeBinders::waterSetUvRepeat(const tbe::Vector2f& v)
{
    if(m_curwater)
    {
        m_curwater->SetUvRepeat(v);
        emit notifyUpdate(m_curwater);
    }
}

void QNodeBinders::waterSetSpeed(double v)
{
    if(m_curwater)
    {
        m_curwater->SetSpeed(v);
        emit notifyUpdate(m_curwater);
    }
}

void QNodeBinders::waterSetBlend(double v)
{
    if(m_curwater)
    {
        m_curwater->SetBlend(v);
        emit notifyUpdate(m_curwater);
    }
}

void QNodeBinders::particleSetGravity(const tbe::Vector3f& v)
{
    if(m_curparticles)
    {
        m_curparticles->SetGravity(v);
        emit notifyUpdate(m_curparticles);
    }
}

void QNodeBinders::particleSetFreemove(double v)
{
    if(m_curparticles)
    {
        m_curparticles->SetFreeMove(v);
        emit notifyUpdate(m_curparticles);
    }
}

void QNodeBinders::particleSetLifeinit(double v)
{
    if(m_curparticles)
    {
        m_curparticles->SetLifeInit(v);
        emit notifyUpdate(m_curparticles);
    }
}

void QNodeBinders::particleSetLifedown(double v)
{
    if(m_curparticles)
    {
        m_curparticles->SetLifeDown(v);
        emit notifyUpdate(m_curparticles);
    }
}

void QNodeBinders::particleSetNumber(int v)
{
    if(m_curparticles)
    {
        m_curparticles->SetNumber(v);
        emit notifyUpdate(m_curparticles);
    }
}

void QNodeBinders::particleSetTexture(const QString& v)
{
    if(m_curparticles)
    {
        m_curparticles->SetTexture(v.toStdString());
        emit notifyUpdate(m_curparticles);
    }
}

void QNodeBinders::particleSetContinousMode(int v)
{
    if(m_curparticles)
    {
        m_curparticles->SetContinousMode(v);
        emit notifyUpdate(m_curparticles);
    }
}

void QNodeBinders::lightSetType(int type)
{
    if(m_curlight)
    {
        m_curlight->SetType((tbe::scene::Light::Type)type);
        emit notifyUpdate(m_curlight);
    }
}

void QNodeBinders::lightSetAmbiant(const tbe::Vector3f& value)
{
    if(m_curlight)
    {
        m_curlight->SetAmbient(vec34(value));
        emit notifyUpdate(m_curlight);
    }
}

void QNodeBinders::lightSetDiffuse(const tbe::Vector3f& value)
{
    if(m_curlight)
    {
        m_curlight->SetDiffuse(vec34(value));
        emit notifyUpdate(m_curlight);
    }
}

void QNodeBinders::lightSetSpecular(const tbe::Vector3f& value)
{
    if(m_curlight)
    {
        m_curlight->SetSpecular(vec34(value));
        emit notifyUpdate(m_curlight);
    }
}

void QNodeBinders::lightSetRadius(double value)
{
    if(m_curlight)
    {
        m_curlight->SetRadius(value);
        emit notifyUpdate(m_curlight);
    }
}
