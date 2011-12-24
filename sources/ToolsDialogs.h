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
#include "ui_drawdialog.h"

#include "QVectorBox.h"

#include "QMeshInteractor.h"

class MaterialEditDialog : public QDialog, public Ui::MaterialEdit
{
public:
    MaterialEditDialog(QWidget* parent);

    QDoubleVector3Box* rgb;
    QIntVector2Box* clipping_framesize;
    QIntVector2Box* clipping_part;

    QMeshInteractor* source_mesh;

    QStandardItemModel* materialsModel;
    QStandardItemModel* textureModel;
};

class DrawToolDialog : public QDialog, public Ui::DrawToolDialog
{
public:
    DrawToolDialog(QWidget* parent);

    QDoubleVector2Box *rotationRange;
    QDoubleVector2Box *scaleRange;

};

#endif	/* TOOLSDIALOGS_H */

