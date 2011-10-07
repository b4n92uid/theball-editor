/* 
 * File:   ClassFactory.cpp
 * Author: b4n92uid
 * 
 * Created on 10 septembre 2011, 12:57
 */

#include "ClassFactory.h"
#include "MainWindow.h"

ClassFactory::ClassFactory(MainWindow* mainwin)
{
    m_mainwin = mainwin;
}

tbe::scene::Mesh* ClassFactory::newMesh(tbe::scene::MeshParallelScene* scene)
{
    QMesh* mesh = new QMesh(m_mainwin);

    m_mainwin->tbeWidget()->meshRegister(mesh);

    return mesh;
}

tbe::scene::Light* ClassFactory::newLight(tbe::scene::LightParallelScene* scene)
{
    QLight* light = new QLight(m_mainwin);

    m_mainwin->tbeWidget()->lightRegister(light);

    return light;
}

tbe::scene::ParticlesEmiter* ClassFactory::newParticles(tbe::scene::ParticlesParallelScene* scene)
{
    QParticles* particles = new QParticles(m_mainwin);

    m_mainwin->tbeWidget()->particlesRegister(particles);

    return particles;
}

tbe::scene::MapMark* ClassFactory::newMapMark(tbe::scene::MapMarkParallelScene* scene)
{
    QMapMark* mapmark = new QMapMark(m_mainwin);

    m_mainwin->tbeWidget()->markRegister(mapmark);

    return mapmark;
}

tbe::scene::Water* ClassFactory::newWater(tbe::scene::WaterParallelScene* scene)
{
    return NULL;
}

void ClassFactory::setupMesh(tbe::scene::Mesh* node)
{
    m_mainwin->tbeWidget()->fetchInterface(node)->setup();
}

void ClassFactory::setupLight(tbe::scene::Light* node)
{
    m_mainwin->tbeWidget()->fetchInterface(node)->setup();
}

void ClassFactory::setupParticles(tbe::scene::ParticlesEmiter* node)
{
    m_mainwin->tbeWidget()->fetchInterface(node)->setup();
}

void ClassFactory::setupMapMark(tbe::scene::MapMark* node)
{
    m_mainwin->tbeWidget()->fetchInterface(node)->setup();
}

void ClassFactory::setupWater(tbe::scene::Water* node)
{
}
