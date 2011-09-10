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
    tbe::scene::Light* newLight(tbe::scene::LightParallelScene* scene);
    tbe::scene::ParticlesEmiter* newParticles(tbe::scene::ParticlesParallelScene* scene);
    tbe::scene::MapMark* newMapMark(tbe::scene::MapMarkParallelScene* scene);
    tbe::scene::Water* newWater(tbe::scene::WaterParallelScene* scene);

    void setupMesh(tbe::scene::Mesh* node);
    void setupLight(tbe::scene::Light* node);
    void setupParticles(tbe::scene::ParticlesEmiter* node);
    void setupMapMark(tbe::scene::MapMark* node);
    void setupWater(tbe::scene::Water* node);

protected:
    MainWindow* m_mainwin;
};
#endif	/* CLASSFACTORY_H */

