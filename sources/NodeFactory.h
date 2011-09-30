/* 
 * File:   NodeFactory.h
 * Author: b4n92uid
 *
 * Created on 29 août 2011, 08:07
 */

#ifndef NODEFACTORY_H
#define	NODEFACTORY_H

#include <Tbe.h>

#include "QMeshInteractor.h"
#include "QParticlesInteractor.h"
#include "QParticlesInteractor.h"
#include "QLightInteractor.h"
#include "QMapMarkInteractor.h"

class MainWindow;

class QMesh : public tbe::scene::Mesh, public QMeshInteractor
{
public:
    QMesh(MainWindow* mainwin);
    QMesh(MainWindow* mainwin, const tbe::scene::Mesh& copy);
    QMesh(const QMesh& copy);
    virtual ~QMesh();
    
    QMesh* clone();
    
    void setup();
};

class QLight : public tbe::scene::Light, public QLightInteractor
{
public:
    QLight(MainWindow* mainwin);
    QLight(MainWindow* mainwin, const tbe::scene::Light& copy);
    QLight(const QLight& copy);
    virtual ~QLight();
    
    QLight* clone();
    
    void setup();
};

class QParticles : public tbe::scene::ParticlesEmiter, public QParticlesInteractor
{
public:
    QParticles(MainWindow* mainwin);
    QParticles(MainWindow* mainwin, const tbe::scene::ParticlesEmiter& copy);
    QParticles(const QParticles& copy);
    virtual ~QParticles();

    QParticles* clone();

    void setup();
};

class QMapMark : public tbe::scene::MapMark, public QMapMarkInteractor
{
public:
    QMapMark(MainWindow* mainwin);
    QMapMark(MainWindow* mainwin, const tbe::scene::MapMark& copy);
    QMapMark(const QMapMark& copy);
    virtual ~QMapMark();
    
    QMapMark* clone();
    
    void setup();
};


#endif	/* NODEFACTORY_H */
