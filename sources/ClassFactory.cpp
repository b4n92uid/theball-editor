/* 
 * File:   ClassFactory.cpp
 * Author: b4n92uid
 * 
 * Created on 10 septembre 2011, 12:57
 */

#include "ClassFactory.h"

#include "QNodeInteractor.h"
#include "QMeshInteractor.h"
#include "QLightInteractor.h"
#include "QParticlesInteractor.h"
#include "QMapMarkInteractor.h"

#include "MainWindow.h"

using namespace tbe;
using namespace scene;

ClassFactory::ClassFactory(MainWindow* mainwin)
{
    m_mainwin = mainwin;
}

tbe::scene::Mesh* ClassFactory::newMesh(tbe::scene::MeshParallelScene* scene)
{
    Mesh* mesh = new Mesh(scene);

    return mesh;
}

QMeshInteractor* ClassFactory::setupMeshGui(tbe::scene::Mesh* mesh)
{
    QMeshInteractor* meshIneractor = new QMeshInteractor(m_mainwin, mesh);

    meshIneractor->setup();

    return meshIneractor;
}

void ClassFactory::setupMesh(tbe::scene::Mesh* mesh)
{
    setupMeshGui(mesh);
}

tbe::scene::Light* ClassFactory::newLight(tbe::scene::MeshParallelScene* scene)
{
    Light* light = new Light(scene);

    return light;
}

void ClassFactory::setupLight(tbe::scene::Light* light)
{
    setupLightGui(light);
}

QLightInteractor* ClassFactory::setupLightGui(tbe::scene::Light* light)
{
    using namespace tbe::scene;

    QLightInteractor* li = new QLightInteractor(m_mainwin, light);

    li->setup();

    return li;
}

tbe::scene::ParticlesEmiter* ClassFactory::newParticles(tbe::scene::ParticlesParallelScene* scene)
{
    ParticlesEmiter* particles = new ParticlesEmiter(scene);

    return particles;
}

void ClassFactory::setupParticles(tbe::scene::ParticlesEmiter* particles)
{
    setupParticlesGui(particles);
}

QParticlesInteractor* ClassFactory::setupParticlesGui(tbe::scene::ParticlesEmiter* particles)
{
    using namespace tbe::scene;

    QParticlesInteractor* pi = new QParticlesInteractor(m_mainwin, particles);

    pi->setup();

    return pi;
}

tbe::scene::MapMark* ClassFactory::newMapMark(tbe::scene::MapMarkParallelScene* scene)
{
    MapMark* mapmark = new MapMark(scene);

    return mapmark;
}

void ClassFactory::setupMapMark(tbe::scene::MapMark* mark)
{
    setupMapMarkGui(mark);
}

QMapMarkInteractor* ClassFactory::setupMapMarkGui(tbe::scene::MapMark* mark)
{
    using namespace tbe::scene;

    QMapMarkInteractor* mi = new QMapMarkInteractor(m_mainwin, mark);

    mi->setup();

    return mi;
}
