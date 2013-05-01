/* 
 * File:   ToolsDialogs.h
 * Author: b4n92uid
 *
 * Created on 18 décembre 2011, 00:40
 */

#ifndef TOOLSDIALOGS_H
#define	TOOLSDIALOGS_H

#include <QtGui/QtGui>

#include <Tbe.h>

#include "ui_matedit.h"

#include "QVectorBox.h"

class QMeshInteractor;

class MaterialEditDialog : public QDialog, public Ui::MaterialEdit
{
    Q_OBJECT

public:
    MaterialEditDialog(QWidget* parent);

    QDoubleVector3Box* ambiant;
    QDoubleVector3Box* diffuse;
    QDoubleVector3Box* specular;

    QDoubleVector3Box* rgb;
    QIntVector2Box* clipping_framesize;
    QIntVector2Box* clipping_part;

    QMeshInteractor* source_mesh;

    QStandardItemModel* textureModel;
};

#endif	/* TOOLSDIALOGS_H */

