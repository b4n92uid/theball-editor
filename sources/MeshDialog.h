/* 
 * File:   MeshDialog.h
 * Author: b4n92uid
 *
 * Created on 23 mai 2013, 17:09
 */

#ifndef MESHDIALOG_H
#define	MESHDIALOG_H

#include <Tbe.h>
#include <QDialog>
#include <QStandardItemModel>
#include "ui_mesh.h"

class QMeshInteractor;
class MaterialDialog;
class QDoubleVector3Box;
class QIntVector2Box;
class MainWindow;

class MeshDialog : public QDialog, public Ui_MeshDialog
{
    Q_OBJECT

public:
    MeshDialog(MainWindow* parent);
    virtual ~MeshDialog();

    void bind(QMeshInteractor* mi);
    void unbind();

    void update(tbe::scene::Mesh* m);
    void updateMaterials(tbe::scene::Mesh* target);

public slots:
    void onApply();

    void onAttachMaterial();
    void onReleaseMaterial();
    void onReloadMaterial();

    void onMaterialSelected();
    void onTextureSelected(const QModelIndex& index);

    void addTexture();
    void delTexture();
    void textureUp();
    void textureDown();

    void textureSetBlendMode();
    void textureSetOrigin();
    void textureSetMipmap();

    void setColor(const tbe::Vector3f& value);
    void setOpacity(double value);

    void setAmbiant(tbe::Vector3f color);
    void setDiffuse(tbe::Vector3f color);
    void setSpecular(tbe::Vector3f color);
    void setShininess(double value);

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
    void setBlendMode();

    void setAlpha(bool stat);
    void setAlphaThreshold(double value);

    void openShaderFileName();
    void releaseShaderFileName();

protected:
    tbe::scene::Material* getSelectedMaterial();
    tbe::scene::SubMesh* getSelectedSubMesh();

private:
    MainWindow* m_mainwin;

    QDoubleVector3Box* ambiant;
    QDoubleVector3Box* diffuse;
    QDoubleVector3Box* specular;

    QDoubleVector3Box* rgb;
    QIntVector2Box* clipping_framesize;
    QIntVector2Box* clipping_part;

    QStandardItemModel* textureModel;

    QMap<QString, QString> m_filepath;

    tbe::scene::Mesh* m_target;
};

#endif	/* MESHDIALOG_H */

