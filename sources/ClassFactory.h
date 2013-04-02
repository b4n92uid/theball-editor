/* 
 * File:   ClassFactory.h
 * Author: b4n92uid
 *
 * Created on 10 septembre 2011, 12:57
 */

#ifndef CLASSFACTORY_H
#define	CLASSFACTORY_H

#include <Tbe.h>

class MainWindow;

class ClassFactory : public tbe::scene::ParserClassFactory
{
public:
    ClassFactory(MainWindow* mainwin);

    tbe::scene::Mesh* newMesh(tbe::scene::MeshParallelScene* scene);
    tbe::scene::Light* newLight(tbe::scene::MeshParallelScene* scene);
    tbe::scene::ParticlesEmiter* newParticles(tbe::scene::ParticlesParallelScene* scene);
    tbe::scene::MapMark* newMapMark(tbe::scene::MapMarkParallelScene* scene);

protected:
    MainWindow* m_mainwin;
};
#endif	/* CLASSFACTORY_H */
