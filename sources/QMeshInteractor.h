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

    QString typeName() const;

public slots:
    void bindWithGui();
    void unbindFromGui();
    void updateGui();

public slots:
    void onMaterialSelected();
    void onTextureSelected(const QModelIndex& index);

    void addTexture();
    void delTexture();
    void textureUp();
    void textureDown();

    void textureSetBlendMode();
    void materialSetBlendMode();

    void setBillBoard();
    void setShadow();

    void setAlphaThreshold(double value);

    void setColor(const tbe::Vector3f& value);
    void setOpacity(double value);

    void setAmbiant(tbe::Vector3f color);
    void setDiffuse(tbe::Vector3f color);
    void setSpecular(tbe::Vector3f color);

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

    void openMaterialDialog();
    void saveMaterialDialog();
    void cancelMaterialDialog();

    void pastMaterials();
    void copyMaterials();

    void setCustomMaterial(bool state);

    tbe::scene::Material* getSelectedMaterial();

private:
    tbe::scene::Mesh* m_target;
    tbe::scene::Mesh* m_undo;
};

#endif	/* QMESHINTERACTOR_H */

