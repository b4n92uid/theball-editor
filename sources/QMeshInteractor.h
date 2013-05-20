/* 
 * File:   QMeshInteractor.h
 * Author: b4n92uid
 *
 * Created on 29 août 2011, 07:23
 */

#ifndef QMESHINTERACTOR_H
#define	QMESHINTERACTOR_H

#include "QNodeInteractor.h"
#include "MaterialDialog.h"

class QMeshInteractor : public QNodeInteractor
{
    Q_OBJECT

public:
    QMeshInteractor(MainWindow* mainwin, tbe::scene::Mesh* target);
    virtual ~QMeshInteractor();

    void setup();
    
    QMeshInteractor* clone();

    QString typeName() const;

public slots:
    void bindWithGui();
    void unbindFromGui();
    void updateGui();

    void setBillBoard();
    void setShadow();
    void setComputeNormal();
    void setComputeTangent();

    void openMaterialDialog();
    void attachMaterial();
    void reloadMaterial();
    void releaseMaterial();

private:
    tbe::scene::Mesh* m_target;
    MaterialDialog* m_materialDialog;
};

#endif	/* QMESHINTERACTOR_H */

