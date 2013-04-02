/* 
 * File:   ToolsDialogs.cpp
 * Author: b4n92uid
 * 
 * Created on 18 décembre 2011, 00:40
 */

#include "ToolsDialogs.h"

#include "QMeshInteractor.h"

MaterialEditDialog::MaterialEditDialog(QWidget* parent) : QDialog(parent)
{
    setupUi(this);

    ambiant = new QDoubleVector3Box(this, ambiant_r, ambiant_g, ambiant_b);
    diffuse = new QDoubleVector3Box(this, diffuse_r, diffuse_g, diffuse_b);
    specular = new QDoubleVector3Box(this, specular_r, specular_g, specular_b);

    rgb = new QDoubleVector3Box(this, color_r, color_g, color_b);
    clipping_framesize = new QIntVector2Box(this, clipping_x, clipping_y);
    clipping_part = new QIntVector2Box(this, clipping_part_x, clipping_part_y);

    source_mesh = NULL;
}
