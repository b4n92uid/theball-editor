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

    virtual QString typeName() const;

public slots:
    void select();
    void deselect();
    void update();

public slots:
    void materialSelected(const QModelIndex& index);
    void textureSelected(const QModelIndex& index);

    void setScale(const tbe::Vector3f& v);

    void addTexture();
    void delTexture();
    void textureUp();
    void textureDown();

    void textureSetBlendMode();
    void materialSetBlendMode();

    void setBillBoard();

    void setAlphaThreshold(double value);

    void setColor(const tbe::Vector3f& value);
    void setOpacity(double value);

    void setFoged(bool stat);
    void setTextured(bool stat);
    void setLighted(bool stat);
    void setCullTrick(bool stat);

    void setTextureClipping(bool state);
    void setTextureFrameSize(const tbe::Vector2i& value);
    void setTexturePart(const tbe::Vector2i& value);
    void setFrameAnimation(bool state);
    void setFrameDuration(int value);

    void setBlend(bool stat);
    void setAlpha(bool stat);

    void openMaterialFile();
    void saveMaterialFile();
    void delMaterialFile();

    void pastMaterials();
    void copyMaterials();

    tbe::scene::Material* getSelectedMaterial();

private:
    tbe::scene::Mesh* m_target;
};

#endif	/* QMESHINTERACTOR_H */

