/* 
 * File:   ToolsDialogs.cpp
 * Author: b4n92uid
 * 
 * Created on 18 décembre 2011, 00:40
 */

#include "ToolsDialogs.h"

MaterialEditDialog::MaterialEditDialog(QWidget* parent) : QDialog(parent)
{
    setupUi(this);

    rgb = new QDoubleVector3Box(this, color_r, color_g, color_b);
    clipping_framesize = new QIntVector2Box(this, clipping_x, clipping_y);
    clipping_part = new QIntVector2Box(this, clipping_part_x, clipping_part_y);

    source_mesh = NULL;
}

DrawToolDialog::DrawToolDialog(QWidget* parent) : QDialog(parent)
{
    setupUi(this);

    rotationRange = new QDoubleVector2Box(this, minRotation, maxRotation);
    scaleRange = new QDoubleVector2Box(this, minScale, maxScale);
}
