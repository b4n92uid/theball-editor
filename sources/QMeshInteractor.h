/* 
 * File:   QMeshInteractor.h
 * Author: b4n92uid
 *
 * Created on 29 août 2011, 07:23
 */

#ifndef QMESHINTERACTOR_H
#define	QMESHINTERACTOR_H

#include "QNodeInteractor.h"

class QMeshInteractor : public QNodeInteractor
{
    Q_OBJECT

public:
    QMeshInteractor(MainWindow* mainwin, tbe::scene::Mesh* target);
    virtual ~QMeshInteractor();

public slots:
    void select();
    void deselect();
    void update();
    
    QMeshInteractor* clone();

public slots:
    void materialSelected(const QModelIndex& index);
    void textureSelected(const QModelIndex& index);

    void addTexture();
    void delTexture();
    void textureUp();
    void textureDown();

    void textureSetBlendMode();
    void materialSetBlendMode();

    void setBillBoard();
    void setOpacity(double value);
    void setAlphaThreshold(double value);

    void setFoged(bool stat);
    void setTextured(bool stat);
    void setLighted(bool stat);
    void setCullTrick(bool stat);

    void setBlend(bool stat);
    void setAlpha(bool stat);

    void setSaveMaterial(bool stat);

    tbe::scene::Material* getSelectedMaterial();

private:
    tbe::scene::Mesh* m_target;
};

#endif	/* QMESHINTERACTOR_H */

