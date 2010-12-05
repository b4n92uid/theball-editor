/* 
 * File:   QVectorBox.cpp
 * Author: b4n92uid
 * 
 * Created on 4 décembre 2010, 00:14
 */

#include "QVectorBox.h"

QVectorBox::QVectorBox(QWidget* parent, QDoubleSpinBox* x, QDoubleSpinBox* y, QDoubleSpinBox* z) : QObject(parent)
{
    m_x = x;
    m_z = y;
    m_y = z;

    connect(m_x, SIGNAL(valueChanged(double)), this, SIGNAL(ComponentValueChanged(double)));
    connect(m_y, SIGNAL(valueChanged(double)), this, SIGNAL(ComponentValueChanged(double)));
    connect(m_z, SIGNAL(valueChanged(double)), this, SIGNAL(ComponentValueChanged(double)));
}

QVectorBox::~QVectorBox()
{
}

tbe::Vector3f QVectorBox::GetVectorValue()
{
    return tbe::Vector3f(m_x->value(), m_y->value(), m_z->value());
}

void QVectorBox::SetVectorValue(const tbe::Vector3f& value)
{
    m_x->setValue(value.x);
    m_y->setValue(value.y);
    m_z->setValue(value.z);
}

void QVectorBox::ComponentValueChanged(double d)
{
    emit valueChanged(GetVectorValue());
}
