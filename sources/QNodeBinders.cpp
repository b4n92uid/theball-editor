/* 
 * File:   QNodeBinders.cpp
 * Author: b4n92uid
 * 
 * Created on 7 décembre 2010, 21:00
 */

#include "QNodeBinders.h"

QWater::QWater(QObject* parent, tbe::scene::Water* water) : QObject(parent)
{
    m_curwater = water;
}

void QWater::SetDeform(double v)
{
    if(m_curwater)
        m_curwater->SetDeform(v);
}

void QWater::SetSize(const tbe::Vector2f& v)
{
    if(m_curwater)
        m_curwater->SetSize(v);
}

void QWater::SetUvRepeat(const tbe::Vector2f& v)
{
    if(m_curwater)
        m_curwater->SetUvRepeat(v);
}

void QWater::SetSpeed(double v)
{
    if(m_curwater)
        m_curwater->SetSpeed(v);
}

void QWater::SetBlend(double v)
{
    if(m_curwater)
        m_curwater->SetBlend(v);
}

void QWater::setCurwater(tbe::scene::Water* curwater)
{
    this->m_curwater = curwater;
}

tbe::scene::Water* QWater::getCurwater() const
{
    return m_curwater;
}

QParticles::QParticles(QObject* parent, tbe::scene::ParticlesEmiter* particles) : QObject(parent)
{
    m_curparticles = particles;
}

void QParticles::setCurparticles(tbe::scene::ParticlesEmiter* curparticles)
{
    this->m_curparticles = curparticles;
}

tbe::scene::ParticlesEmiter* QParticles::getCurparticles() const
{
    return m_curparticles;
}

void QParticles::SetGravity(const tbe::Vector3f& v)
{
    if(m_curparticles)
        m_curparticles->SetGravity(v);
}

void QParticles::SetFreemove(double v)
{
    if(m_curparticles)
        m_curparticles->SetFreeMove(v);
}

void QParticles::SetLifeinit(double v)
{
    if(m_curparticles)
        m_curparticles->SetLifeInit(v);
}

void QParticles::SetLifedown(double v)
{
    if(m_curparticles)
        m_curparticles->SetLifeDown(v);
}

void QParticles::SetNumber(int v)
{
    if(m_curparticles)
        m_curparticles->SetNumber(v);
}

void QParticles::SetTexture(const QString& v)
{
    if(m_curparticles)
        m_curparticles->SetTexture(v.toStdString());
}

void QParticles::SetContinousMode(int v)
{
    if(m_curparticles)
        m_curparticles->SetContinousMode(v);
}

QMesh::QMesh(QObject* parent, tbe::scene::Mesh* mesh)
{
    m_curmesh = mesh;
}

void QMesh::setCurmesh(tbe::scene::Mesh* curmesh)
{
    this->m_curmesh = curmesh;
}

tbe::scene::Mesh* QMesh::getCurmesh() const
{
    return m_curmesh;
}
