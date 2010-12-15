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
    m_curNode = NULL;
}

void QNodeBinders::setCurmesh(tbe::scene::Mesh* curmesh)
{
    m_curNode = m_curmesh = curmesh;
    m_curwater = NULL;
    m_curparticles = NULL;
}

tbe::scene::Mesh* QNodeBinders::getCurmesh() const
{
    return m_curmesh;
}

void QNodeBinders::setCurNode(tbe::scene::Node* curNode)
{
    m_curNode = curNode;
    m_curmesh = NULL;
    m_curparticles = NULL;
    m_curwater = NULL;

    emit notifyUpdate(m_curNode);
}

tbe::scene::Node* QNodeBinders::getCurNode() const
{
    return m_curNode;
}

void QNodeBinders::setCurwater(tbe::scene::Water* curwater)
{
    m_curNode = m_curwater = curwater;
    m_curmesh = NULL;
    m_curparticles = NULL;

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

    emit notifyUpdate(m_curparticles);
}

tbe::scene::ParticlesEmiter* QNodeBinders::getCurparticles() const
{
    return m_curparticles;
}

void QNodeBinders::SetName(const QString& s)
{
    if(m_curNode)
    {
        m_curNode->SetName(s.toStdString());
        emit notifyUpdate(m_curNode);
    }
}

void QNodeBinders::SetPos(const tbe::Vector3f& v)
{
    if(m_curNode)
    {
        m_curNode->SetPos(v);
        emit notifyUpdate(m_curNode);
    }
}

void QNodeBinders::SetMatrix(const tbe::Matrix4f& m)
{
    if(m_curNode)
    {
        m_curNode->SetMatrix(m);
        emit notifyUpdate(m_curNode);
    }
}

void QNodeBinders::SetDeform(double v)
{
    if(m_curwater)
    {
        m_curwater->SetDeform(v);
        emit notifyUpdate(m_curwater);
    }
}

void QNodeBinders::SetSize(const tbe::Vector2f& v)
{
    if(m_curwater)
    {
        m_curwater->SetSize(v);
        emit notifyUpdate(m_curwater);
    }
}

void QNodeBinders::SetUvRepeat(const tbe::Vector2f& v)
{
    if(m_curwater)
    {
        m_curwater->SetUvRepeat(v);
        emit notifyUpdate(m_curwater);
    }
}

void QNodeBinders::SetSpeed(double v)
{
    if(m_curwater)
    {
        m_curwater->SetSpeed(v);
        emit notifyUpdate(m_curwater);
    }
}

void QNodeBinders::SetBlend(double v)
{
    if(m_curwater)
    {
        m_curwater->SetBlend(v);
        emit notifyUpdate(m_curwater);
    }
}

void QNodeBinders::SetGravity(const tbe::Vector3f& v)
{
    if(m_curparticles)
    {
        m_curparticles->SetGravity(v);
        emit notifyUpdate(m_curparticles);
    }
}

void QNodeBinders::SetFreemove(double v)
{
    if(m_curparticles)
    {
        m_curparticles->SetFreeMove(v);
        emit notifyUpdate(m_curparticles);
    }
}

void QNodeBinders::SetLifeinit(double v)
{
    if(m_curparticles)
    {
        m_curparticles->SetLifeInit(v);
        emit notifyUpdate(m_curparticles);
    }
}

void QNodeBinders::SetLifedown(double v)
{
    if(m_curparticles)
    {
        m_curparticles->SetLifeDown(v);
        emit notifyUpdate(m_curparticles);
    }
}

void QNodeBinders::SetNumber(int v)
{
    if(m_curparticles)
    {
        m_curparticles->SetNumber(v);
        emit notifyUpdate(m_curparticles);
    }
}

void QNodeBinders::SetTexture(const QString& v)
{
    if(m_curparticles)
    {
        m_curparticles->SetTexture(v.toStdString());
        emit notifyUpdate(m_curparticles);
    }
}

void QNodeBinders::SetContinousMode(int v)
{
    if(m_curparticles)
    {
        m_curparticles->SetContinousMode(v);
        emit notifyUpdate(m_curparticles);
    }
}
