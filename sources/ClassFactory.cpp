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

    return mesh;
}

tbe::scene::Light* ClassFactory::newLight(tbe::scene::LightParallelScene* scene)
{
    QLight* light = new QLight(m_mainwin);

    return light;
}

tbe::scene::ParticlesEmiter* ClassFactory::newParticles(tbe::scene::ParticlesParallelScene* scene)
{
    QParticles* particles = new QParticles(m_mainwin);

    return particles;
}

tbe::scene::MapMark* ClassFactory::newMapMark(tbe::scene::MapMarkParallelScene* scene)
{
    QMapMark* mapmark = new QMapMark(m_mainwin);

    return mapmark;
}
