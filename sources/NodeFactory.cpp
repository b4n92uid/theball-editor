/* 
 * File:   NodeFactory.cpp
 * Author: b4n92uid
 * 
 * Created on 29 août 2011, 08:07
 */

#include "NodeFactory.h"

#include "QMeshInteractor.h"
#include "QParticlesInteractor.h"
#include "QParticlesInteractor.h"
#include "QLightInteractor.h"
#include "QMapMarkInteractor.h"

#include "MainWindow.h"

using namespace tbe::scene;

QMesh::QMesh(MainWindow* mainwin) :
Mesh(mainwin->getTbeWidget()->getSceneParser()->getMeshScene()),
QMeshInteractor(mainwin, this)
{
}

QMesh::QMesh(MainWindow* mainwin, const tbe::scene::Mesh& copy) :
Mesh(copy), QMeshInteractor(mainwin, this)
{
}

QMesh::~QMesh()
{
}

QLight::QLight(MainWindow* mainwin) :
Light(mainwin->getTbeWidget()->getSceneParser()->getLightScene()),
QLightInteractor(mainwin, this)
{
}

QLight::QLight(MainWindow* mainwin, const tbe::scene::Light& copy) :
Light(copy), QLightInteractor(mainwin, this)
{

}

QLight::~QLight()
{
}

QParticles::QParticles(MainWindow* mainwin) :
ParticlesEmiter(mainwin->getTbeWidget()->getSceneParser()->getParticlesScene()),
QParticlesInteractor(mainwin, this)
{
}

QParticles::QParticles(MainWindow* mainwin, const tbe::scene::ParticlesEmiter& copy) :
ParticlesEmiter(copy), QParticlesInteractor(mainwin, this)
{
}

QParticles::~QParticles()
{
}

QMapMark::QMapMark(MainWindow* mainwin) :
MapMark(mainwin->getTbeWidget()->getSceneParser()->getMarkScene()),
QMapMarkInteractor(mainwin, this)
{
}

QMapMark::QMapMark(MainWindow* mainwin, const tbe::scene::MapMark& copy) :
MapMark(copy), QMapMarkInteractor(mainwin, this)
{

}

QMapMark::~QMapMark()
{
}
