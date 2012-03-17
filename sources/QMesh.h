/* 
 * File:   QMesh.h
 * Author: b4n92uid
 *
 * Created on 17 mars 2012, 18:42
 */

#ifndef QMESH_H
#define	QMESH_H

#include <Tbe.h>
#include "QMeshInteractor.h"

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

#endif	/* QMESH_H */
