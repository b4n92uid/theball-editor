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

class NodeFactory
{
public:
    NodeFactory(MainWindow* parent);
    virtual ~NodeFactory();

private:
    MainWindow* m_mainwin;
};

class QMesh : public tbe::scene::Mesh, public QMeshInteractor
{
public:
    QMesh(MainWindow* mainwin);
    QMesh(MainWindow* mainwin, const tbe::scene::Mesh& copy);
    virtual ~QMesh();
private:

};

class QLight : public tbe::scene::Light, public QLightInteractor
{
public:
    QLight(MainWindow* mainwin);
    QLight(MainWindow* mainwin, const tbe::scene::Light& copy);
    virtual ~QLight();
private:

};

class QParticles : public tbe::scene::ParticlesEmiter, public QParticlesInteractor
{
public:
    QParticles(MainWindow* mainwin);
    QParticles(MainWindow* mainwin, const tbe::scene::ParticlesEmiter& copy);
    virtual ~QParticles();
private:

};

class QMapMark : public tbe::scene::MapMark, public QMapMarkInteractor
{
public:
    QMapMark(MainWindow* mainwin);
    QMapMark(MainWindow* mainwin, const tbe::scene::MapMark& copy);
    virtual ~QMapMark();
private:

};


#endif	/* NODEFACTORY_H */
