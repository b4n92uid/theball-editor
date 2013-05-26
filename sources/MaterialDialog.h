/* 
 * File:   MaterialDialog.h
 * Author: b4n92uid
 *
 * Created on 18 décembre 2011, 00:40
 */

#ifndef MaterialDialog_H
#define	MaterialDialog_H

#include <QtGui/QtGui>

#include <Tbe.h>

#include "ui_matedit.h"

#include "QVectorBox.h"

class QMeshInteractor;
class MainWindow;

class MaterialDialog : public QDialog, protected Ui::MaterialEdit
{
    Q_OBJECT

public:
    MaterialDialog(MainWindow* parent);
    ~MaterialDialog();

    void bind();
    void unbind();

    void update(tbe::scene::Mesh* target, QString filepath);

public slots:
    void onApply();
    void onClose();

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

protected:
    QDoubleVector3Box* ambiant;
    QDoubleVector3Box* diffuse;
    QDoubleVector3Box* specular;

    QDoubleVector3Box* rgb;
    QIntVector2Box* clipping_framesize;
    QIntVector2Box* clipping_part;

    QStandardItemModel* textureModel;

    QString m_filepath;

    tbe::scene::Mesh* m_target;
    MainWindow* m_mainwin;
};

#endif	/* MaterialDialog_H */

