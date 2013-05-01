/* 
 * File:   ClassFactory.h
 * Author: b4n92uid
 *
 * Created on 10 septembre 2011, 12:57
 */

#ifndef CLASSFACTORY_H
#define	CLASSFACTORY_H

#include <Tbe.h>

#include "QMeshInteractor.h"
#include "QLightInteractor.h"
#include "QParticlesInteractor.h"
#include "QMapMarkInteractor.h"

class MainWindow;

class ClassFactory : public tbe::scene::ParserClassFactory
{
public:
    ClassFactory(MainWindow* mainwin);

    tbe::scene::Mesh* newMesh(tbe::scene::MeshParallelScene* scene);
    void setupMesh(tbe::scene::Mesh* mesh);
    QMeshInteractor* setupMeshGui(tbe::scene::Mesh* mesh);

    tbe::scene::Light* newLight(tbe::scene::MeshParallelScene* scene);
    void setupLight(tbe::scene::Light* light);
    QLightInteractor* setupLightGui(tbe::scene::Light* light);

    tbe::scene::ParticlesEmiter* newParticles(tbe::scene::ParticlesParallelScene* scene);
    void setupParticles(tbe::scene::ParticlesEmiter* particles);
    QParticlesInteractor* setupParticlesGui(tbe::scene::ParticlesEmiter* particles);

    tbe::scene::MapMark* newMapMark(tbe::scene::MapMarkParallelScene* scene);
    void setupMapMark(tbe::scene::MapMark* mark);
    QMapMarkInteractor* setupMapMarkGui(tbe::scene::MapMark* mark);

protected:
    MainWindow* m_mainwin;
};
#endif	/* CLASSFACTORY_H */
